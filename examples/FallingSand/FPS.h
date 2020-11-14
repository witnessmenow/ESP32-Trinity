#ifndef FPS_h
#define FPS_h
/*******************************************************************
    FPS
    Tries to precisely calculate the number of tick() calls per second
    Assumes more than 1FPS
    It's not trying to be light weight

    Written by @tetra3dprint
    Twiter: https://twitter.com/tetra3dprint
    Github: https://github.com/tetra3dprint
    
 *******************************************************************/
//------------------------------------------------------------------------------------------------------------------
class FPS
{
    //----------------------------------------------
  private:
    //----------------------------------------------
    char fpsStr[10];
    float FPSActual;
    int FrameCount;
    long currentMillis;
    long startMillis;
    long passedMillis;
    const long oneThou = 1000;
    const float oneThouF = 1000;
    //----------------------------------------------
  public:
    //----------------------------------------------
    FPS() {

    }
    //----------------------------------------------
    void tick()
    {
      FrameCount++;
      currentMillis = millis();
      passedMillis = currentMillis - startMillis;
      if (passedMillis > oneThou) {        
        long remainder = passedMillis - oneThou;
        startMillis = currentMillis + remainder; // already started the next 1000 millis frame
        float FPM = (float)FrameCount / (float)passedMillis;
        FrameCount = FPM * (float)remainder; // number of frames already rendered in the next 1000 millis frame        
        FPSActual = FPM * oneThouF; // adjust the FPS to exactly 1000 millis
      }
    }
    //----------------------------------------------
    void getReady()
    {
      FrameCount = 0;
      FPSActual = 0;
      startMillis = millis();
    }
    //----------------------------------------------
    String getStringFPS()
    {
      sprintf(fpsStr, "%.2f", FPSActual);
      return fpsStr;
    }
    //----------------------------------------------
    float getFloatFPS()
    {
      return FPSActual;
    }
    //----------------------------------------------

    //------------------------------------------------------------------------------------------------------------------
};
#endif
