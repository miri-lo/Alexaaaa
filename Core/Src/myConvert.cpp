#include "myConvert.h"
#include "mfcc.h"
#include "kws.h"

#include <string>
//extern "C" {

#define MFCC_IN_SIZE 800
#define MFCC_STEP_SIZE 320
#define MFCC_OUT_SIZE 10

MFCC* mfcc = new MFCC(MFCC_OUT_SIZE, MFCC_IN_SIZE, 2);

KWS* kws = new KWS();

float mfccInputFrame[MFCC_IN_SIZE]; // input frame of mfcc
q7_t mfccOutTmp[MFCC_OUT_SIZE]; // result of mfcc
float mfccOut[MFCC_OUT_SIZE]; // result after typecast

extern int16_t safedValues[PCM_SAMPLES_PER_SEC];

void myConvert() {
// Feature extraction
//printf("Feature extraction started\r\n");
int feature_counter = 0;
for (int i = 0; i < 16000-MFCC_STEP_SIZE; i+=MFCC_STEP_SIZE, feature_counter++) {
  for (int j = 0; j < MFCC_IN_SIZE; j++) {
    if (i+j >= 16000)
      mfccInputFrame[j] = 0;
    else {
      mfccInputFrame[j] = (float) safedValues[i+j]; //rec->get(i+j);
      //printf("%f, ", mfccInputFrame[j]);
    }
  }
  mfcc->mfcc_compute(mfccInputFrame, mfccOutTmp);
  for (int k = 0; k < MFCC_OUT_SIZE; k++) {
    kws->mMFCC[feature_counter][k] = (ai_float) mfccOutTmp[k];
  }
  int index = kws->runInference();
  std::string word = kws->indexToWord(index);
  printf("Recognized word is ", word);
  // switsch on/off if word detected
}

//printf("Feature extraction ended\r\n");
}

//}
