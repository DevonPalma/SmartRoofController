class Sampler {
    int sampleMaxSize;
    int sampleCurSize;
    int sampleRawValue;

    int lastSampleTotal;
    int lastSampleAdded;

  public:

    Sampler(int maxSample) {
      sampleMaxSize = maxSample;
    }

    void addSample(int newSample) {
      sampleCurSize++;
      sampleRawValue += newSample;
      lastSampleAdded = newSample;

      if (sampleCurSize >= sampleMaxSize) {
        lastSampleTotal = sampleRawValue / sampleCurSize;
        sampleRawValue = 0;
        sampleCurSize = 0;
      }
    }

    int getLastSampleAdded() {
      return lastSampleAdded;
    }

    int getSample() {
      return lastSampleTotal;
    }

    int getCurrentSample() {
      if (sampleCurSize == 0) {
        return 0;
      }
      return sampleRawValue / sampleCurSize;
    }
};
