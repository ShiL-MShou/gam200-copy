/* Start Header
*****************************************************************/
/*!
\file ObjectAllocator.cpp
\author Kok Zhe Yan, zheyan.kok, 390002118
\par zheyan.kok\@digipen.edu
\date January 23, 2020
\brief
This file contains the implemention of a class named ObjectAllocator which will
be used to allocate and deallocate fixed-sized memory blocks for a client.
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header
*******************************************************************/

#include "stdafx.h"
#include "ObjectAllocator.h"
#include <cstring>      // memset

using uchar = unsigned char;

ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig& config)
: _Config(config), _Stats(), _PageList(nullptr), _FreeList(nullptr)
{
  _Stats.ObjectSize_ = ObjectSize;
  // Calculate left and inter alignment
  if (_Config.Alignment_ > 1)
  {
    _Config.LeftAlignSize_ = static_cast<unsigned>(_Config.Alignment_ - (sizeof(void*) + _Config.PadBytes_ + _Config.HBlockInfo_.size_) % _Config.Alignment_);
      
    _Config.InterAlignSize_ = static_cast<unsigned>(_Config.Alignment_ - (ObjectSize + _Config.PadBytes_ * 2ll + _Config.HBlockInfo_.size_) % _Config.Alignment_);
    
    if (_Config.LeftAlignSize_ == _Config.Alignment_)
      _Config.LeftAlignSize_ = 0;
    
    if (_Config.InterAlignSize_ == _Config.Alignment_)
      _Config.InterAlignSize_ = 0;
    
  }
  // Calculate page size
  _Stats.PageSize_ = _Config.ObjectsPerPage_ * ( ObjectSize 
     + _Config.PadBytes_ * 2ll + _Config.InterAlignSize_ + _Config.HBlockInfo_.size_ ) 
     - _Config.InterAlignSize_ + _Config.LeftAlignSize_ + sizeof(void*);

  if (_Config.UseCPPMemManager_ == false)
  {
    try
    {
      AddNewPage();
    }
    catch(OAException exception)
    {
      throw(exception);
    }
  }
}

ObjectAllocator::~ObjectAllocator() throw()
{
  uchar* page;
  while (_PageList)
  {
    page = reinterpret_cast<uchar*>(_PageList);
    _PageList = _PageList->Next;
    delete[] page;
  }
}


void ObjectAllocator::AddNewPage(GenericObject* _nextpage)
{
  uchar* address;
  try
  {
    address = new uchar[_Stats.PageSize_];
  }
  catch(std::bad_alloc &)
  {
    throw(OAException(OAException::E_NO_MEMORY, "System is out of memory."));
  }
  
  _PageList = reinterpret_cast<GenericObject*>(address);
  _PageList->Next = _nextpage;
  address += sizeof(void*);
  ++_Stats.PagesInUse_; 
  
  GenericObject* nextList;
  
  for (unsigned i = 0; i < _Config.ObjectsPerPage_; ++i)
  {
    // Get address to proper alignment
    // First Block
    if (i == 0)
    {
      if (_Config.DebugOn_ == true)
      {
        std::memset(address, ALIGN_PATTERN, _Config.LeftAlignSize_);
      }
      if (i < _Config.ObjectsPerPage_ - 1)
      {
        address += _Config.LeftAlignSize_;
      }
    }
    else // Subsequent Blocks
    {
      if (_Config.DebugOn_ == true)
      {
        std::memset(address, ALIGN_PATTERN, _Config.InterAlignSize_);
      }
      address += _Config.InterAlignSize_;
    }
    
    // Add headerblocks size
    if (_Config.DebugOn_ == true)
    {
      std::memset(address, 0, _Config.HBlockInfo_.size_);
    }
    *address = 0;
    address += _Config.HBlockInfo_.size_;
    
    // Add left padding
    if (_Config.DebugOn_ == true)
    {
      std::memset(address, PAD_PATTERN, _Config.PadBytes_);
    }
    address += _Config.PadBytes_;
    
     // Add data bytes and set new free list
    if (_Config.DebugOn_ == true)
    {
      std::memset(address, UNALLOCATED_PATTERN, _Stats.ObjectSize_);  
    }
    
    nextList = _FreeList;
    _FreeList = reinterpret_cast<GenericObject*>(address);
    ++_Stats.FreeObjects_;
    _FreeList->Next = nextList; 
    address += _Stats.ObjectSize_;
    
    // Add right padding
    if (_Config.DebugOn_ == true)
    {
      std::memset(address, PAD_PATTERN, _Config.PadBytes_); 
    }
    address += _Config.PadBytes_;
  }

}


void *ObjectAllocator::Allocate(const char *label)
{
    UNREFERENCED_PARAMETER(label);
  if (_Config.UseCPPMemManager_ == true)
  {
    uchar* address;
    try
    {
      address = new uchar[_Stats.ObjectSize_];
    }
    catch(const std::bad_alloc &)
    {
      throw(OAException(OAException::E_NO_MEMORY, "System is out of memory."));
    }
    ++_Stats.ObjectsInUse_;
    if (_Stats.ObjectsInUse_ > _Stats.MostObjects_)
    {
      ++_Stats.MostObjects_;
    }
    ++_Stats.Allocations_;
    --_Stats.FreeObjects_;
    return address;
  }
  
  if (_Stats.PagesInUse_ == 0)
  {
    try
    {
      AddNewPage(); 
    }
    catch(OAException exception)
    {
      throw(exception);
    }
  }
  else if (_Stats.FreeObjects_ == 0)
  {
    if (_Config.MaxPages_ == 0 || _Stats.PagesInUse_ < _Config.MaxPages_)
    {
      try
      {
        AddNewPage(_PageList); 
      }
      catch(OAException exception)
      {
        throw(exception);
      }
    }
    else
    {
      throw(OAException(OAException::E_NO_PAGES, "Maximum page count has been reached."));
    }
  }
  
  // allocating of memory
  uchar* newmemory = reinterpret_cast<uchar*>(_FreeList);
  _FreeList = _FreeList->Next;
  //if (_Config.DebugOn_ == true)
  //{
  //  std::memset(newmemory, ALLOCATED_PATTERN, _Stats.ObjectSize_);
  //}
  //if (_Config.HBlockInfo_.size_ != 0)
  //{
  //  if (_Config.HBlockInfo_.type_ == OAConfig::hbBasic)
  //  {  
  //    uchar* flag = newmemory - _Config.PadBytes_ - sizeof(uchar);
  //    *flag = static_cast<uchar>(1);
  //    unsigned int* alloc = reinterpret_cast<unsigned int*>(newmemory - _Config.PadBytes_ - _Config.HBlockInfo_.size_);
  //    *alloc = _Stats.Allocations_ + 1;
  //    
  //  }
  //  else if (_Config.HBlockInfo_.type_ == OAConfig::hbExtended)
  //  {
  //    uchar* flag = newmemory - _Config.PadBytes_ - sizeof(uchar);
  //    *flag = static_cast<uchar>(1);
  //    unsigned short* counter = reinterpret_cast<unsigned short*>(
  //      newmemory - _Config.PadBytes_ - _Config.HBlockInfo_.size_
  //      + _Config.HBlockInfo_.additional_);
  //    ++*counter;
  //    unsigned int* alloc = reinterpret_cast<unsigned int*>(newmemory - _Config.PadBytes_ - _Config.HBlockInfo_.size_
  //      + _Config.HBlockInfo_.additional_ + sizeof(unsigned short));
  //    *alloc = _Stats.Allocations_ + 1;
  //  }
  //  else
  //  {
  //    MemBlockInfo** externheader = reinterpret_cast<MemBlockInfo**>(newmemory - _Config.PadBytes_ - _Config.HBlockInfo_.size_);
  //    *externheader = new MemBlockInfo{true, _Stats.Allocations_ + 1, label ? new char[strlen(label) + 1] : nullptr};
  //    /*if (label)
  //      strcpy_s((*externheader)->label, label);*/
  //  }
  //  
  //}
  ++_Stats.ObjectsInUse_;
  if (_Stats.ObjectsInUse_ > _Stats.MostObjects_)
  {
    ++_Stats.MostObjects_;
  }
  ++_Stats.Allocations_;
  --_Stats.FreeObjects_;
  
  return newmemory;
  
}

void ObjectAllocator::Free(void *Object)
{
  if (_Config.UseCPPMemManager_ == true)
  {
    uchar* memoryblock = reinterpret_cast<uchar*>(Object);
    delete[] memoryblock;

    --_Stats.ObjectsInUse_;
    ++_Stats.Deallocations_;
    ++_Stats.FreeObjects_;

    return;
  }
  
  GenericObject* pagelist = _PageList;
  uchar* address = reinterpret_cast<uchar*>(Object);
  
  // Performing checks if debug is on
  if (_Config.DebugOn_ == true)
  {
    bool onBoundary = false;
    bool isFreed = true;
    bool isCorrupted = false;

    while (pagelist != nullptr)
    {
      // Check if object is within the list of pages
      if ( reinterpret_cast<uchar*>(pagelist) < address
        && reinterpret_cast<uchar*>(pagelist) + _Stats.PageSize_ > address)
      {
        uchar* datastart = reinterpret_cast<uchar*>(pagelist) + sizeof(void*) + 
            _Config.LeftAlignSize_ + _Config.PadBytes_ + _Config.HBlockInfo_.size_;
        size_t dataseparation = _Stats.ObjectSize_ + _Config.PadBytes_ * 2ll + 
                            _Config.InterAlignSize_ + _Config.HBlockInfo_.size_;
        // Check if object is on a block boundary
        if ((address - datastart) % dataseparation == 0)
        {
          onBoundary = true;
        }
        else
        {
          onBoundary = false;
          break;
        }
      }
      pagelist = pagelist->Next;
    }

    if (onBoundary == false)
    {
      throw(OAException(OAException::E_BAD_BOUNDARY, "Object not on any block boundary."));
    }
    else
    {
      // Check data block if it is freed
      for (unsigned i = sizeof(uchar*); i < _Stats.ObjectSize_; ++i)
      {
        if ( address[i] != FREED_PATTERN )
        {
          isFreed = false;
          break;
        }
      }

      // Check padding blocks
      uchar* leftpadding = address - _Config.PadBytes_;
      uchar* rightpadding = address + _Stats.ObjectSize_;
      for (unsigned i = 0; i < _Config.PadBytes_; ++i)
      {
        if (leftpadding[i] != PAD_PATTERN || rightpadding[i] != PAD_PATTERN)
        {
          isCorrupted = true;
          break;
        }
      }
    }
    
    if (isCorrupted == true)
    {
      throw(OAException(OAException::E_CORRUPTED_BLOCK, "Block has been corrupted."));
    }
    else if (isFreed == true)
    {
      throw(OAException(OAException::E_MULTIPLE_FREE, "Block has already been freed."));
    }
  }
  
  // Freeing of Memory
  
  if (_Config.HBlockInfo_.size_ != 0)
  {
    if (_Config.HBlockInfo_.type_ == OAConfig::hbBasic)
    {  
      uchar* flag = address - _Config.PadBytes_ - sizeof(uchar);
      *flag = 0;
      unsigned int* alloc = reinterpret_cast<unsigned int*>(address - _Config.PadBytes_ - _Config.HBlockInfo_.size_);
      *alloc = 0;
    }
    else if (_Config.HBlockInfo_.type_ == OAConfig::hbExtended)
    {
      uchar* flag = address - _Config.PadBytes_ - sizeof(uchar);
      *flag = 0;
      unsigned int* alloc = reinterpret_cast<unsigned int*>(address - _Config.PadBytes_ - _Config.HBlockInfo_.size_
        + _Config.HBlockInfo_.additional_ + sizeof(unsigned short));
      *alloc = 0;
    }
    else
    {
      MemBlockInfo** alloc = reinterpret_cast<MemBlockInfo **>(address - _Config.PadBytes_ - _Config.HBlockInfo_.size_);
      //delete[] (*alloc)->label;
     // (*alloc)->label = nullptr;
      delete *alloc;
      *alloc = 0;
    }
  }
  
  if (_Config.DebugOn_ == true)
  {
    std::memset(Object, FREED_PATTERN, _Stats.ObjectSize_);
  }
  GenericObject* memoryblock = reinterpret_cast<GenericObject*>(Object);
  memoryblock->Next = _FreeList;
  _FreeList = memoryblock;

  --_Stats.ObjectsInUse_;
  ++_Stats.Deallocations_;
  ++_Stats.FreeObjects_;
}

unsigned ObjectAllocator::DumpMemoryInUse(DUMPCALLBACK fn) const
{
  GenericObject* pagelist = _PageList;
  uchar* memoryblock;
  
  while (pagelist)
  {
    memoryblock = reinterpret_cast<uchar*>(pagelist) + sizeof(void*) 
    + _Config.LeftAlignSize_ + _Config.HBlockInfo_.size_ + _Config.PadBytes_;
    
    for (unsigned i = 0; i < _Config.ObjectsPerPage_; ++i)
    {
      if (*memoryblock == ALLOCATED_PATTERN)
      {
        fn(memoryblock, _Stats.ObjectSize_);
      }
      if (i < _Config.ObjectsPerPage_ - 1)
      {
        memoryblock += _Stats.ObjectSize_ + _Config.PadBytes_ * 2ll
        + _Config.InterAlignSize_ + _Config.HBlockInfo_.size_;    
      }
    }
    pagelist = pagelist->Next;
  }
  return _Stats.ObjectsInUse_;
}

unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const
{
  if (_Config.PadBytes_ == 0 || _Config.DebugOn_ == false)
  {
    return 0;
  }

  unsigned corruptedBlocks = 0;
  GenericObject* pagelist = _PageList;
  uchar* memoryblock;
  uchar* leftpadding;
  uchar* rightpadding;

  while (pagelist)
  {
    memoryblock = reinterpret_cast<uchar*>(pagelist) + sizeof(void*) 
    + _Config.LeftAlignSize_ + _Config.HBlockInfo_.size_ + _Config.PadBytes_;
    
    for (unsigned i = 0; i < _Config.ObjectsPerPage_; ++i)
    {
      // Check padding blocks
      leftpadding = memoryblock - _Config.PadBytes_;
      rightpadding = memoryblock + _Stats.ObjectSize_;
      for (unsigned j = 0; j < _Config.PadBytes_; ++j)
      {
        if (leftpadding[j] != PAD_PATTERN || rightpadding[j] != PAD_PATTERN)
        {
          fn(memoryblock, _Stats.ObjectSize_);
          ++corruptedBlocks;  
          break;
        }
      }

      // Prevents passing the end of allocated memory
      if (i < _Config.ObjectsPerPage_ - 1)
      {
        memoryblock += _Stats.ObjectSize_ + _Config.PadBytes_ * 2ll
        + _Config.InterAlignSize_ + _Config.HBlockInfo_.size_;  
      }  
    }
    pagelist = pagelist->Next;
  }

  return corruptedBlocks;
}

 unsigned ObjectAllocator::FreeEmptyPages()
 {
  unsigned emptypages = 0;
  GenericObject* pagelist = _PageList;
  uchar* memoryblock;

  if (pagelist == nullptr)
  {
    return emptypages;
  }
  
  // Delete the first page if it is empty and move on 
  // till first page is not empty
  while (pagelist && IsEmpty(pagelist))
  {
    ++emptypages;
    --_Stats.PagesInUse_;
    RemovePage(pagelist);
    memoryblock = reinterpret_cast<uchar*>(pagelist);
    pagelist = pagelist->Next;
    _PageList = pagelist;
    delete[] memoryblock;
  }
  // Delete the next page if it is empty and moves on till no more pages
  // is found
  while (pagelist)
  {
    while (pagelist->Next && IsEmpty(pagelist->Next))
    {
      ++emptypages;
      --_Stats.PagesInUse_;
      RemovePage(pagelist->Next);
      memoryblock = reinterpret_cast<uchar*>(pagelist->Next);
      pagelist->Next = pagelist->Next->Next;
      delete[] memoryblock;
    }
    pagelist = pagelist->Next;
  }
  return emptypages;
}

 void ObjectAllocator::Clear()
 {
     uchar* page;
     while (_PageList)
     {
         page = reinterpret_cast<uchar*>(_PageList);
         _PageList = _PageList->Next;
         delete[] page;
     }
     OAStats newstats;
     newstats.ObjectSize_ = _Stats.ObjectSize_;
     newstats.PageSize_ = _Stats.PageSize_;
     _Stats = newstats;
 }

bool ObjectAllocator::IsEmpty(GenericObject* _page)
{
  uchar* block = reinterpret_cast<uchar*>(_page) + sizeof(void*) + 
    _Config.LeftAlignSize_ + _Config.PadBytes_ + _Config.HBlockInfo_.size_;
  size_t dataseparation = _Stats.ObjectSize_ + _Config.PadBytes_ * 2ll + 
    _Config.InterAlignSize_ + _Config.HBlockInfo_.size_;

  // Go through the blocks in the page
  for (unsigned i = 0; i < _Config.ObjectsPerPage_; ++i)
  { 
    // If any block is not on the Free List, page is not empty
    if (InFreeList(block) == false)
    {
      return false;
    }
    
    if (i < _Config.ObjectsPerPage_ - 1)
    {
      block += dataseparation;  
    }
  }

  return true;
}

bool ObjectAllocator::InFreeList(unsigned char* _block)
{
  GenericObject* currFreeBlock = _FreeList;
  
  // Searches through the freelist
  while (currFreeBlock != nullptr)
  {
    if (_block == reinterpret_cast<uchar*>(currFreeBlock))
    {
      return true;
    }
    
    currFreeBlock = currFreeBlock->Next;
  }
  // Block is not found in FreeList
  return false;
}

void ObjectAllocator::RemovePage(GenericObject* _page)
{
  uchar* curraddress = reinterpret_cast<uchar*>(_page) + sizeof(void*) + 
    _Config.LeftAlignSize_ + _Config.PadBytes_ + _Config.HBlockInfo_.size_;
  size_t dataseparation = _Stats.ObjectSize_ + _Config.PadBytes_ * 2ll + 
    _Config.InterAlignSize_ + _Config.HBlockInfo_.size_;
  
   // Go through the blocks in the page
  for (unsigned i = 0; i < _Config.ObjectsPerPage_; ++i)
  { 
    GenericObject* currBlock = reinterpret_cast<GenericObject*>(curraddress);
    GenericObject* currFreeBlock = _FreeList;
    
    // if the block to be removed from freelist is the 1st block
    if (currFreeBlock && currBlock == currFreeBlock)
    {
      _FreeList = _FreeList->Next;
      --_Stats.FreeObjects_;
    }
    else
    {
      while (currFreeBlock != nullptr)
      {
        if (currFreeBlock->Next && currBlock == currFreeBlock->Next)
        {
          currFreeBlock->Next = currFreeBlock->Next->Next;
          --_Stats.FreeObjects_;
          break;
        }
        else
          currFreeBlock = currFreeBlock->Next;
      }
    }
    
    if (i < _Config.ObjectsPerPage_ - 1)
    {
      curraddress += dataseparation;
    }
  }
}

void ObjectAllocator::SetDebugState(bool State)   
{
  _Config.DebugOn_ = State;
}

const void * ObjectAllocator::GetFreeList() const 
{
  return _FreeList;
}

const void * ObjectAllocator::GetPageList() const  
{
  return _PageList;
}

OAConfig ObjectAllocator::GetConfig() const       
{
  return _Config;
}

OAStats ObjectAllocator::GetStats() const        
{
  return _Stats;
}




