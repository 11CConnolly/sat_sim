#include <static/checksum.h>

/*
 * Provides the XOR checksum executed on the bytes of a given buffer
 * 
 * In - pointer to data buffer
 * In - length in bytes of data to XOR
 * 
 * Return - returns byte of XOR checksum
 */
uint8_t compute_XOR_checksum(char* buff, int length)
{
  uint8_t result = 0;

  /* Quick robustness NULL check */
  if (buff) 
  {
    /* Go through each byte */
    for (int i = 0; i < length; i++)
    {
      result ^= buff[i];
    }
  }

  return result;
}
