#ifndef MEM_VERIFY_H_
#define MEM_VERIFY_H_

#ifdef __cplusplus
extern "C" {
#endif

bool TMEM_Verify(alt_u32 BaseAddr, alt_u32 ByteLen, alt_u32 InitValue, bool bShowMessage);
bool TMEM_QuickVerify(alt_u32 BaseAddr, alt_u32 DataSize, alt_u32 DataWidth, alt_u32 AddrWidth);

#ifdef __cplusplus
}
#endif


#endif /*MEM_VERIFY_H_*/
