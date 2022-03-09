class Sampler {
    int sampleCurSize;
    int sampleRawValue;

  public:

    Sampler() {}

    void addSample(int newSample) {
      sampleCurSize++;
      sampleRawValue += newSample;
    }

    int getSample() {
      if (sampleCurSize == 0) {
        return 0;
      }
      
      int readVal = sampleRawValue / sampleCurSize;

      sampleRawValue = 0;
      sampleCurSize = 0;

      return readVal;
    }
};
