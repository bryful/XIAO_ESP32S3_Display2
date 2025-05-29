#include "FsSerial.hpp"
bool BrySerial::CreateHeader(byte *head, const char *header, uint32_t size)
{
  if (strlen(header) < 4)
    return false;
  head[0] = 't';
  head[1] = 'e';
  head[2] = 'x';
  head[3] = 't';
  head[4] = head[5] = head[6] = head[7] = '\0';
  for (int i = 0; i < 4; ++i)
  {
    head[i] = (byte)(header[i]);
    head[i + 4] = (byte)((size >> (i * 8)) & 0xFF);
  }
  return true;
}
void BrySerial::SendText(const char *str)
{
  uint32_t  len = strlen(str);
  if (len < 0)
    return;
  byte head[8];
  if (CreateHeader(head, "text", len) == false)
    return;
  Serial.write((const char *)head, 8);
  Serial.write(str, len);
  Serial.flush();
}
void BrySerial::SendBin(const char *key, uint8_t *data, uint32_t size)
{
  byte head[8];
  if (CreateHeader(head, key, size) == false)
    return;

  Serial.write((const char *)head, 8);
  Serial.write(data, size);
  Serial.flush();
}
bool BrySerial::compHeader(const char *head, const char *header)
{
  bool ret = false;
  if ((strlen(header) < 4) || (strlen(head) < 4))
    return ret;
  ret = ((head[0] == header[0]) && (head[1] == header[1]) && (head[2] == header[2]) && (head[3] == header[3]));
  return ret;
}