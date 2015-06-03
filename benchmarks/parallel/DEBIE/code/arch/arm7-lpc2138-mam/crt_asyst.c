typedef unsigned char byte;
typedef unsigned int uint;
typedef void (*pfunc)(void);

int main(int argc, char *argv[]);
extern byte __ASYST_DATA_LOAD[];
extern byte __ASYST_DATA_START[];
extern byte __ASYST_DATA_END[];
extern byte __ASYST_BSS_START[];
extern byte __ASYST_BSS_END[]; 
extern pfunc __ASYST_CTOR_START[]; 
extern pfunc __ASYST_CTOR_END[];
extern pfunc __ASYST_DTOR_START[]; 
extern pfunc __ASYST_DTOR_END[];
extern byte __ASYST_HEAP_START[];

#define __ASYST_IS_ALIGNED(P) ((((uint)(P)) & (~(sizeof(uint) -1))) == ((uint)(P)))

void __asyst_memset_byte(byte *pbyDest, byte byValue, uint uiSize)
{
  byte *pbyDestLast=pbyDest+uiSize;
  for(; pbyDest!=pbyDestLast; pbyDest++)
    pbyDest[0]=byValue;
}

void __asyst_memset_uint(uint *puiDest, uint uiValue, uint uiSize)
{
  uint *puiDestLast=puiDest+uiSize;
  for(; puiDest!=puiDestLast; puiDest++)
    puiDest[0]=uiValue;
}

void __asyst_memset(byte *pbyDest, byte byValue, uint uiSize)
{
  if(__ASYST_IS_ALIGNED(pbyDest) &&
     __ASYST_IS_ALIGNED(uiSize))
  {
    uint uiValue=0;
    __asyst_memset_byte((byte*)&uiValue, byValue, sizeof(uint));
    return __asyst_memset_uint((uint*)pbyDest, uiValue, uiSize/sizeof(uint));  
  }
  else
    return __asyst_memset_byte(pbyDest, byValue, uiSize);
  
}

void *__asyst_memcpy_uint(uint *puiDest, const uint *puiSrc, uint uiSize)
{
  uint *puiDestLast=puiDest+uiSize;
  for(; puiDest!=puiDestLast; puiSrc++,puiDest++)
    puiDest[0]=puiSrc[0];
  return puiDestLast;
}

void *__asyst_memcpy_byte(byte *pbyDest, const byte *pbySrc, uint uiSize)
{
  byte *pbyDestLast=pbyDest+uiSize;
  for(; pbyDest!=pbyDestLast; pbySrc++,pbyDest++)
    pbyDest[0]=pbySrc[0];
  return pbyDestLast;    
}

void *__asyst_memcpy(byte *pbyDest, const byte *pbySrc, uint uiSize)
{
  if(__ASYST_IS_ALIGNED(pbyDest) &&
     __ASYST_IS_ALIGNED(pbySrc) &&
     __ASYST_IS_ALIGNED(uiSize))
    return __asyst_memcpy_uint((uint*)pbyDest, (uint*)pbySrc, uiSize/sizeof(uint));
  else
    return __asyst_memcpy_byte(pbyDest, pbySrc, uiSize);    
}

void __asyst_main()
{
  pfunc *pfTable;
  // copy data
  if(__ASYST_DATA_START != __ASYST_DATA_LOAD)
    __asyst_memcpy(__ASYST_DATA_START, __ASYST_DATA_LOAD, __ASYST_DATA_END - __ASYST_DATA_START);
  // zero bss
  __asyst_memset(__ASYST_BSS_START, 0, __ASYST_BSS_END - __ASYST_BSS_START);
  
  /* Not used in C project
  // call constructors
  for(pfTable=__ASYST_CTOR_END-1; pfTable!=__ASYST_CTOR_START; pfTable--)
    pfTable[0]();
  */
  
  // main
  main(0, 0);
  
  /* Not used in C project
  // call destructors
  //for(pfTable=__ASYST_DTOR_START; pfTable!=__ASYST_DTOR_END; pfTable++)
    //pfTable[0]();
  */
  
  while(1);
}
