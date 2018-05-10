set(HEADER lbpFeature.h lbpFeatureSvm.h multiImg.h pcaDimReduce.h ycbcr.h dataset.h lbp.h detect.h)
set(SOURCE lbpFeature.cpp lbpFeatureSvm.cpp multiImg.cpp pcaDimReduce.cpp ycbcr.cpp
  detect_face.cpp dataset.cc)

add_library(lbp_svm ${SOURCE} ${HEADER})
target_link_libraries(lbp_svm PRIVATE ${OpenCV_LIBS} ${Boost_LIBRARIES} fmt::fmt-header-only)

add_executable(detect detect.cc ${HEADER} )
target_link_libraries(detect lbp_svm fmt::fmt-header-only)

