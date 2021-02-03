
/******************************************************************************
* Name:    CRC-8               x8+x2+x+1
* Poly:    0x07
* Init:    0x00
* Note:
*****************************************************************************/
uint8_t crc8(uint8_t *data, uint8_t length)
{
  uint8_t i;
  uint8_t crc = 0;        // Initial value
  while(length--)
  {
    crc ^= *data++;        // crc ^= *data; data++;
    for ( i = 0; i < 8; i++ )
    {
      if ( crc & 0x80 )
        crc = (crc << 1) ^ 0x07;
      else
        crc <<= 1;
    }
  }
  return crc;
}