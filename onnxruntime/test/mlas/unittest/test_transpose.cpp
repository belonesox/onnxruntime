// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "test_util.h"

template <typename ElementType, bool Threaded>
class MlasTransposeTest : public MlasTestBase {
 private:
  MatrixGuardBuffer<ElementType> BufferInput;
  MatrixGuardBuffer<ElementType> BufferOutput;
  MatrixGuardBuffer<ElementType> BufferOutputReference;
  MLAS_THREADPOOL* threadpool_;

  void
  Test(size_t M, size_t N) {
    ElementType* Input = BufferInput.GetBuffer(M * N);
    ElementType* Output = BufferOutput.GetBuffer(M * N);
    ElementType* OutputReference = BufferOutputReference.GetBuffer(M * N);

    MlasTranspose(Input, Output, M, N, threadpool_);
    ReferenceTranspose(Input, OutputReference, M, N);

    ASSERT_EQ(memcmp(Output, OutputReference, M * N * sizeof(ElementType)), 0) << " [" << M << "," << N << "]";
  }

  void ReferenceTranspose(const ElementType* Input, ElementType* Output, size_t M, size_t N) {
    for (size_t m = 0; m < M; m++) {
      for (size_t n = 0; n < N; n++) {
        Output[n * M + m] = Input[m * N + n];
      }
    }
  }

 public:
  MlasTransposeTest() : threadpool_(Threaded ? GetMlasThreadPool() : nullptr) {}

  static const char* GetTestSuiteName() {
    static const std::string suite_name = std::string("Transpose_") +
                                          GetTypeString() +
                                          std::string(Threaded ? "_Threaded" : "_SingleThread");
    return suite_name.c_str();
  }

  static const std::string GetTypeString() {
    if (std::is_same<ElementType, float>::value) return std::string("FP32");
    if (std::is_same<ElementType, uint32_t>::value) return std::string("U32");
    if (std::is_same<ElementType, uint16_t>::value) return std::string("U16");
    if (std::is_same<ElementType, uint8_t>::value) return std::string("U8");
    return std::string("unknown");
  }

  void ExecuteShort(void) override {
    for (size_t m = 1; m <= 32; m++) {
      for (size_t n = 1; n <= 32; n++) {
        Test(m, n);
      }
    }
  }
};

static UNUSED_VARIABLE bool added_to_main = AddTestRegister([](bool is_short_execute) {
  size_t count = 0;
  if (is_short_execute) {
    count += MlasDirectShortExecuteTests<MlasTransposeTest<uint32_t, false>>::RegisterShortExecute();
    count += MlasDirectShortExecuteTests<MlasTransposeTest<uint16_t, false>>::RegisterShortExecute();
    count += MlasDirectShortExecuteTests<MlasTransposeTest<uint8_t, false>>::RegisterShortExecute();
    count += MlasDirectShortExecuteTests<MlasTransposeTest<uint32_t, true>>::RegisterShortExecute();
    count += MlasDirectShortExecuteTests<MlasTransposeTest<uint16_t, true>>::RegisterShortExecute();
    count += MlasDirectShortExecuteTests<MlasTransposeTest<uint8_t, true>>::RegisterShortExecute();
    count += MlasDirectShortExecuteTests<MlasTransposeTest<float, true>>::RegisterShortExecute();
    count += MlasDirectShortExecuteTests<MlasTransposeTest<int8_t, true>>::RegisterShortExecute();
  }
  return count;
});
