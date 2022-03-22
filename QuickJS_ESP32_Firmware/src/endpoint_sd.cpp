#include <Arduino.h>
#include "main_config.h"

#include "endpoint_types.h"
#include "endpoint_sd.h"
#include "module_utils.h"
#include <SD.h>

long endp_sd_exists(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;
  bool ret = SD.exists(fname);

  response["result"] = ret;

  return 0;
}

long endp_sd_mkdir(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;
  bool ret = SD.mkdir(fname);

  response["result"] = ret;
  
  return 0;
}

long endp_sd_remove(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;
  bool ret = SD.remove(fname);

  response["result"] = ret;
  
  return 0;
}

long endp_sd_rmdir(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;
  bool ret = SD.rmdir(fname);

  response["result"] = ret;
  
  return 0;
}

long endp_sd_size(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;
  File file = SD.open(fname);
  if( !file )
    return -1;
  uint32_t size = file.size();
  file.close();

  response["result"] = size;
  
  return 0;
}

long endp_sd_readBinary(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;
  File file = SD.open(fname, FILE_READ);
  if( !file )
    return -1;
  uint32_t fsize = file.size();
  int32_t offset = request["offset"] | 0;
  if( offset < 0 )
    offset = 0;
  if( offset > fsize )
    offset = fsize;
  int32_t size = request["size"] | -1;
  if( size < 0 || offset + size > fsize )
    size = fsize - offset;

  unsigned char *buffer = (unsigned char*)malloc(size);
  if( buffer == NULL ){
    file.close();
    return -1;
  }
  file.seek(offset);
  file.read(buffer, size);
  file.close();

  uint32_t enclen = b64_encode_length(size);
  char *encstr = (char*)malloc(enclen + 1);
  b64_encode(buffer, size, encstr);
  encstr[enclen] = '\0';

  response["result"] = encstr;

  free(encstr);
  free(buffer);

  return 0;
}

long endp_sd_writeBinary(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;

  File file = SD.open(fname, FILE_WRITE);
  if( !file )
    return -1;

  const char *encstr = request["buffer"];
  if( encstr == NULL ){
    file.close();
    return -1;
  }

  uint32_t bsize = b64_decode_length(encstr);
  unsigned char *p_buffer = (unsigned char*)malloc(bsize);
  b64_decode(encstr, p_buffer);

  uint32_t fsize = file.size();
  int32_t offset = request["offset"] | -1;
  if( offset < 0 )
    offset = fsize;
  int32_t size = request["size"] | -1;
  if( size < 0 || size > bsize )
    size = bsize;

  file.seek(offset);
  size_t wrote = file.write(p_buffer, size);
  file.close();

  free(p_buffer);

  response["result"] = wrote;

  return 0;
}


long endp_sd_readText(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;
  File file = SD.open(fname, FILE_READ);
  if( !file )
    return -1;

  uint32_t fsize = file.size();
  char *p_buffer = (char*)malloc(fsize + 1);
  if( p_buffer == NULL ){
    file.close();
    return -1;
  }
  file.readBytes(p_buffer, fsize);
  file.close();
  p_buffer[fsize] = '\0';

  response["result"] = p_buffer;
  free(p_buffer);

  return 0;
}

long endp_sd_writeText(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;

  File file = SD.open(fname, FILE_WRITE);
  if( !file )
    return -1;

  const char *str = request["text"];
  if( str == NULL ){
    file.close();
    return -1;
  }
  size_t size = strlen(str);
  if( magic == 1 )
    file.seek(file.size());
  size_t wrote = file.write((const uint8_t*)str, size);
  file.close();
  if( wrote != size )
    return -1;

  response["result"] = wrote;

  return 0;
}

long endp_sd_isDirectory(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;
  File file = SD.open(fname);
  if( !file )
    return -1;
  
  bool ret = file.isDirectory();
  file.close();

  response["result"] = ret;

  return 0;
}

long endp_sd_list(JsonObject request, JsonObject response, int magic)
{
  const char *fname = request["filename"];
  if( fname == NULL )
    return -1;
  File base = SD.open(fname);
  if( !base )
    return -1;

  JsonArray arry = response.createNestedArray("result");
  File file;
  for( int i = 0 ; file = base.openNextFile(); i++ ){
    arry[i] = (char*)file.name();
    file.close();
  }
  base.close();

  return 0;
}

EndpointEntry sd_table[] = {
  EndpointEntry{ endp_sd_exists, "/sd-exists", -1 },
  EndpointEntry{ endp_sd_mkdir, "/sd-mkdir", -1 },
  EndpointEntry{ endp_sd_remove, "/sd-remove", -1 },
  EndpointEntry{ endp_sd_rmdir, "/sd-rmdir", -1 },
  EndpointEntry{ endp_sd_size, "/sd-size", -1 },
  EndpointEntry{ endp_sd_readBinary, "/sd-readBinary", -1 },
  EndpointEntry{ endp_sd_writeBinary, "/sd-writeBinary", -1 },
  EndpointEntry{ endp_sd_readText, "/sd-readText", -1 },
  EndpointEntry{ endp_sd_writeText, "/sd-writeText", 0 },
  EndpointEntry{ endp_sd_writeText, "/sd-appendText", 1 },
  EndpointEntry{ endp_sd_isDirectory, "/sd-isDirectory", -1 },
  EndpointEntry{ endp_sd_list, "/sd-list", -1 },
};

const int num_of_sd_entry = sizeof(sd_table) / sizeof(EndpointEntry);
