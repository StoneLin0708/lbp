set(HEADER lbpFeature.h lbpFeatureSvm.h multiImg.h pcaDimReduce.h ycbcr.h dataset.h)
set(SOURCE lbpFeature.cpp lbpFeatureSvm.cpp multiImg.cpp pcaDimReduce.cpp ycbcr.cpp
  detect_face.cpp lbp_svm.cpp)

add_executable(lbp_svm ${SOURCE} ${HEADER})
target_link_libraries(lbp_svm ${OpenCV_LIBS} ${Boost_LIBRARIES} fmt::fmt-header-only)

