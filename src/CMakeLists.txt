set(HEADER lbpFeature.h lbpFeatureSvm.h multiImg.h pcaDimReduce.h ycbcr.h)
set(SOURCE lbpFeature.cpp lbpFeatureSvm.cpp multiImg.cpp pcaDimReduce.cpp ycbcr.cpp
  detectMultiScale.cpp lbp_svm.cpp)

add_executable(lbp_svm ${SOURCE} ${HEADER})
target_link_libraries(lbp_svm PRIVATE ${OpenCV_LIBS} fmt::fmt-header-only)

