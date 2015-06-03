void begin(void);
void FBCore(float r[256], float y[256], float H[8][32], float F[8][32]);

int numiters = 2;

int main(void)
{
  begin();
  return 0;
}

void begin(void)
{
  float r[256];
  float y[256];
  float H[8][32];
  float F[8][32];
  
  int i, j;

  _Pragma( "loopbound min 256 max 256" )
  for (i = 0;i < 256;i++)
    r[i] = i + 1;

  _Pragma( "loopbound min 32 max 32" )
  for (i = 0;i < 32;i++) {

    _Pragma( "loopbound min 8 max 8" )
    for (j = 0;j < 8;j++) {
      H[j][i] = i * 32 + j * 8 + j + i + j + 1;
      F[j][i] = i * j + j * j + j + i;
    }
  }

  _Pragma( "loopbound min 2 max 2" )
  while (numiters-- > 0) {
    FBCore(r, y, H, F);
  }
}

// the FB core gets the input vector (r) , the filter responses H and F and 
// generates the output vector(y)
void FBCore(float r[256], float y[256], float H[8][32], float F[8][32])
{
  int i, j, k;
  
  _Pragma( "loopbound min 256 max 256" )
  for (i = 0; i < 256;i++)
    y[i] = 0;

  _Pragma( "loopbound min 8 max 8" )
  for (i = 0; i < 8; i++) {
    float Vect_H[256]; //(output of the H)
    float Vect_Dn[(int) 256 / 8]; //output of the down sampler;
    float Vect_Up[256]; // output of the up sampler;
    float Vect_F[256]; // this is the output of the

    //convolving H
    _Pragma( "loopbound min 256 max 256" )
    for (j = 0; j < 256; j++) {
      Vect_H[j] = 0;
      _Pragma( "loopbound min 1 max 32" )
      for (k = 0; ((k < 32) & ((j - k) >= 0)); k++) {
        Vect_H[j] += H[i][k] * r[j - k];
      }
    }

    //Down Sampling
    _Pragma( "loopbound min 32 max 32" )
    for (j = 0; j < 256 / 8; j++)
      Vect_Dn[j] = Vect_H[j * 8];

    //Up Sampling
    _Pragma( "loopbound min 256 max 256" )
    for (j = 0; j < 256;j++)
      Vect_Up[j] = 0;
    _Pragma( "loopbound min 32 max 32" )
    for (j = 0; j < 256 / 8;j++)
      Vect_Up[j*8] = Vect_Dn[j];

    //convolving F
    _Pragma( "loopbound min 256 max 256" )
    for (j = 0; j < 256; j++) {
      Vect_F[j] = 0;
      _Pragma( "loopbound min 1 max 32" )
      for (k = 0; ((k < 32) & ((j - k) >= 0)); k++) {
        Vect_F[j] += F[i][k] * Vect_Up[j - k];
      }
    }

    //adding the results to the y matrix

    _Pragma( "loopbound min 256 max 256" )
    for (j = 0; j < 256; j++) {
      y[j] += Vect_F[j];
    }
  }
}
