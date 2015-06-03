
void __memcpy(void *a, const void *b, int c) 
{
  char *dest = (char*) a;
  char *source = (char*) b;
  int copied;
  _Pragma( "loopbound min 4 max 4" )
  for(copied = 0; copied < c; copied++) {
    *dest = *source; 
    dest++;
    source++;
  }
}
