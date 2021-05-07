//cv::VideoCapture("C:/Users/rphil/Desktop/Orbital/Content/bata-park/video.mp4");


#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>
#include <string>



struct Track {
    std::vector<cv::Rect> PointByFrame;
};

void print(std::string text) {
    std::cout << text.c_str() << std::endl;
}



int main(int argc, char** argv)
{

    bool use_mask = false;


    cv::Mat img;

    cv::Mat searchAreaImg;
    cv::Mat patternAreaImg;

    cv::Rect searchAreaRect;
    cv::Rect patternAreaRect; // Relative to search area

    cv::Mat trackPathOverlay;
    cv::Mat mask;


    const char* image_window = "Source Image";
    const char* result_window = "Result window";

    int match_method = cv::TM_SQDIFF_NORMED;

    double maxThd = 0.7;
    double minThd = 0.3;


    cv::VideoCapture video;
    Track trck;


    video = cv::VideoCapture("C:/Users/rphil/Desktop/Orbital/Content/bata-park/video.mp4");
    video.read(img);

    //1. Search area
    //2. Pattern area

    cv::Rect tmpRect = cv::selectROI(img);

    img(tmpRect).copyTo(patternAreaImg);

    trackPathOverlay = cv::Mat(img.rows, img.cols, img.type(), cv::Scalar(0, 0, 0, 0));
    
    
    searchAreaRect = cv::Rect(
        cv::Point(tmpRect.tl().x - 50, tmpRect.tl().y - 50),
        cv::Point(tmpRect.br().x + 50, tmpRect.br().y + 50)
    );


    patternAreaRect = cv::Rect(
        tmpRect.x - searchAreaRect.x,
        tmpRect.y - searchAreaRect.y,
        tmpRect.width,
        tmpRect.height
    );


    trck.PointByFrame.push_back(patternAreaRect);

    std::cout << "Pattern area:" << patternAreaRect << std::endl;



    std::cout << "Search area:" << searchAreaRect << std::endl;

    cv::namedWindow(image_window, cv::WINDOW_AUTOSIZE);

    int searchAreaUpdateSkip = 2;
    int patternAreaUpdateSkip = 2;

    // Start from frame 2
    while (video.read(img)) {

        // update search area every x frames
        if (searchAreaUpdateSkip-- == 0) {
            searchAreaUpdateSkip = 2;

            // Adjust search are to pattern area
            // Adjust the center of the search area to the center of the tracking area
            cv::Point2d centerSearchArea =
                cv::Point2d(
                    (searchAreaRect.tl().x + searchAreaRect.br().x) / 2,
                    (searchAreaRect.tl().y + searchAreaRect.br().y) / 2
                );

            cv::Point2d centerPatternArea =
                cv::Point2d(
                    (patternAreaRect.tl().x + patternAreaRect.br().x) / 2,
                    (patternAreaRect.tl().y + patternAreaRect.br().y) / 2
                );

            double dx = centerPatternArea.x - centerSearchArea.x;
            double dy = centerPatternArea.y - centerSearchArea.y;

            searchAreaRect.x += dx;
            searchAreaRect.y += dy;


        }

        if (patternAreaUpdateSkip-- == 2) {
            patternAreaUpdateSkip = 2;

            //cv::Mat currPatternAreaImg;
            //img(patternAreaRect).copyTo(currPatternAreaImg);

            //cv::Mat res;
            //cv::compare(patternAreaImg, currPatternAreaImg, res, cv::CMP_EQ);

            //double amountPixels = patternAreaImg.cols * patternAreaImg.rows;

            //// Black/Zero is a mismatched pixel

            //double amoutBlack = 0;
            //res.forEach<cv::Point3_<uint8_t>>(
            //    [&](cv::Point3_<uint8_t>& pixel, const int position[]) -> void {
            //        if (pixel.x == 0 && pixel.y == 0 && pixel.z == 0) {
            //            amoutBlack++;
            //        }
            //    });

            //double percentage = (amoutBlack / amountPixels) * 100;

            //std::cout << percentage << std::endl;

            



            // TODO: keep first pattern area and compare similarities


        }

        // First time: get pattern area from frame 2

        {
            if (searchAreaRect.x < 0) {
                searchAreaRect.x = 0;
            }
            if (searchAreaRect.y < 0) {
                searchAreaRect.y = 0;
            }
            if (searchAreaRect.x + searchAreaRect.width > img.cols) {
                searchAreaRect.x = img.cols - searchAreaRect.width;
            }
            if (searchAreaRect.y + searchAreaRect.height > img.rows) {
                searchAreaRect.y = img.rows - searchAreaRect.height;
            }


            if (patternAreaRect.x <= 0) {
                break;
            }
            if (patternAreaRect.y <= 0) {
                break;
            }
            if (patternAreaRect.x + patternAreaRect.width >= img.cols) {
                break;
            }
            if (patternAreaRect.y + patternAreaRect.height >= img.rows) {
                break;
            }


        }
        



        img(searchAreaRect).copyTo(searchAreaImg);

        cv::Point matchLoc = cv::Point(0, 0);

        {

            /*
                TM_SQDIFF        = 0, //!< \f[R(x,y)= \sum _{x',y'} (T(x',y')-I(x+x',y+y'))^2\f]
                TM_SQDIFF_NORMED = 1, //!< \f[R(x,y)= \frac{\sum_{x',y'} (T(x',y')-I(x+x',y+y'))^2}{\sqrt{\sum_{x',y'}T(x',y')^2 \cdot \sum_{x',y'} I(x+x',y+y')^2}}\f]
                TM_CCORR         = 2, //!< \f[R(x,y)= \sum _{x',y'} (T(x',y')  \cdot I(x+x',y+y'))\f]
                TM_CCORR_NORMED  = 3, //!< \f[R(x,y)= \frac{\sum_{x',y'} (T(x',y') \cdot I(x+x',y+y'))}{\sqrt{\sum_{x',y'}T(x',y')^2 \cdot \sum_{x',y'} I(x+x',y+y')^2}}\f]
                TM_CCOEFF        = 4, //!< \f[R(x,y)= \sum _{x',y'} (T'(x',y')  \cdot I'(x+x',y+y'))\f]
                          //!< where
                          //!< \f[\begin{array}{l} T'(x',y')=T(x',y') - 1/(w  \cdot h)  \cdot \sum _{x'',y''} T(x'',y'') \\ I'(x+x',y+y')=I(x+x',y+y') - 1/(w  \cdot h)  \cdot \sum _{x'',y''} I(x+x'',y+y'') \end{array}\f]
                TM_CCOEFF_NORMED = 5  //!< \f[R(x,y)= \frac{ \sum_{x',y'} (T'(x',y') \cdot I'(x+x',y+y')) }{ \sqrt{\sum_{x',y'}T'(x',y')^2 \cdot \sum_{x',y'} I'(x+x',y+y')^2} }\f]
            */



            cv::Mat result;
            bool method_accepts_mask = (cv::TM_SQDIFF == match_method || match_method == cv::TM_CCORR_NORMED);
            if (use_mask && method_accepts_mask)
            {

                /** @example samples/cpp/tutorial_code/Histograms_Matching/MatchTemplate_Demo.cpp
                An example using Template Matching algorithm
                */

                /** @brief Compares a template against overlapped image regions.

                The function slides through image , compares the overlapped patches of size \f$w \times h\f$ against
                templ using the specified method and stores the comparison results in result . Here are the formulae
                for the available comparison methods ( \f$I\f$ denotes image, \f$T\f$ template, \f$R\f$ result ). The summation
                is done over template and/or the image patch: \f$x' = 0...w-1, y' = 0...h-1\f$

                After the function finishes the comparison, the best matches can be found as global minimums (when
                #TM_SQDIFF was used) or maximums (when #TM_CCORR or #TM_CCOEFF was used) using the
                #minMaxLoc function. In case of a color image, template summation in the numerator and each sum in
                the denominator is done over all of the channels and separate mean values are used for each channel.
                That is, the function can take a color template and a color image. The result will still be a
                single-channel image, which is easier to analyze.

                @param image Image where the search is running. It must be 8-bit or 32-bit floating-point.
                @param templ Searched template. It must be not greater than the source image and have the same
                data type.
                @param result Map of comparison results. It must be single-channel 32-bit floating-point. If image
                is \f$W \times H\f$ and templ is \f$w \times h\f$ , then result is \f$(W-w+1) \times (H-h+1)\f$ .
                @param method Parameter specifying the comparison method, see #TemplateMatchModes
                @param mask Mask of searched template. It must have the same datatype and size with templ. It is
                not set by default. Currently, only the #TM_SQDIFF and #TM_CCORR_NORMED methods are supported.
                */

                cv::matchTemplate(searchAreaImg, patternAreaImg, result, match_method, mask);
            }
            else
            {
                // First time: search template from frame 1 in frame 2 in search area
                cv::matchTemplate(searchAreaImg, patternAreaImg, result, match_method);
            }

            cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

            double minVal = 0.0;
            double maxVal = 0.0;

            cv::Point minLoc = cv::Point(-1, -1);
            cv::Point maxLoc = cv::Point(-1, -1);

            /** @brief Finds the global minimum and maximum in an array.

            The function cv::minMaxLoc finds the minimum and maximum element values and their positions. The
            extremums are searched across the whole array or, if mask is not an empty array, in the specified
            array region.

            The function do not work with multi-channel arrays. If you need to find minimum or maximum
            elements across all the channels, use Mat::reshape first to reinterpret the array as
            single-channel. Or you may extract the particular channel using either extractImageCOI , or
            mixChannels , or split .
            @param src input single-channel array.
            @param minVal pointer to the returned minimum value; NULL is used if not required.
            @param maxVal pointer to the returned maximum value; NULL is used if not required.
            @param minLoc pointer to the returned minimum location (in 2D case); NULL is used if not required.
            @param maxLoc pointer to the returned maximum location (in 2D case); NULL is used if not required.
            @param mask optional mask used to select a sub-array.
            @sa max, min, compare, inRange, extractImageCOI, mixChannels, split, Mat::reshape
            */

            cv::minMaxLoc(
                result,
                &minVal, // Not used
                &maxVal, // Not used
                &minLoc, // used for tl corner (used)
                &maxLoc, // used for tl corner
                cv::Mat());

            std::cout << "Result minval:" << minVal << std::endl;
            std::cout << "Result maxVal:" << maxVal << std::endl;
            std::cout << "Result minLoc:" << minLoc << std::endl;
            std::cout << "Result maxLoc:" << maxLoc << std::endl;

            double dConfidence = 0.0;

            // update template
            if (match_method == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED)
            {
                dConfidence = 1 - minVal;
                if (minVal < minThd) {
                    print("Choose minLoc");
                    matchLoc = minLoc;
                }
                else {
                    break; // threshold passed: match not good enough
                }
            }
            else
            {
                dConfidence = maxVal;
                if (maxVal > maxThd) {
                    print("Choose maxLoc");
                    matchLoc = maxLoc;
                }
                else {
                    break; // threshold passed: match not good enough
                }
            }

            print("Confidence:" + std::to_string(dConfidence));

        }


        // First time: get new pattern area rect on frame 2
        cv::Rect newPatternAreaRect =
            cv::Rect(
                cv::Point(
                    searchAreaRect.x + matchLoc.x,
                    searchAreaRect.y + matchLoc.y
                ), // tl
                cv::Point(
                    searchAreaRect.x + matchLoc.x + patternAreaImg.cols,
                    searchAreaRect.y + matchLoc.y + patternAreaImg.rows
                ) // br
            );

        trck.PointByFrame.push_back(newPatternAreaRect);


        cv::Mat img_display;
        img.copyTo(img_display);

        // Ui stuff
        {

            cv::Point currTrackPos = cv::Point(
                (newPatternAreaRect.tl().x + newPatternAreaRect.br().x) / 2,
                (newPatternAreaRect.tl().y + newPatternAreaRect.br().y) / 2);

            static cv::Point lastTrackPos = cv::Point(
                (newPatternAreaRect.tl().x + newPatternAreaRect.br().x) / 2,
                (newPatternAreaRect.tl().y + newPatternAreaRect.br().y) / 2);

            // Draw track path
            cv::circle(trackPathOverlay,
                currTrackPos,
                1,
                cv::Scalar(255.0, 255.0, 0.0, 0.0),
                3
            );
            cv::line(trackPathOverlay, lastTrackPos, currTrackPos, cv::Scalar(255.0, 255.0, 0.0, 0.0));

            lastTrackPos = currTrackPos;


            cv::addWeighted(img_display, 1.0, trackPathOverlay, 1.0, 0.0, img_display);

            

            // draw pattern area with center

            cv::circle(img_display,
                cv::Point(
                    (newPatternAreaRect.tl().x + newPatternAreaRect.br().x) / 2,
                    (newPatternAreaRect.tl().y + newPatternAreaRect.br().y) / 2),
                3,
                cv::Scalar(255.0, 0.0, 255.0, 0.0),
                1
            );
            cv::rectangle(
                img_display,
                newPatternAreaRect.tl(),
                newPatternAreaRect.br(),
                cv::Scalar(255.0, 0.0, 0.0, 0.0),
                2,
                8,
                0
            );

            // draw search area

            cv::rectangle(
                img_display,
                searchAreaRect.tl(),
                searchAreaRect.br(),
                cv::Scalar(0.0, 255.0, 255.0, 0.0),
                2,
                8,
                0
            );


            cv::imshow(image_window, img_display);

            cv::waitKey(5);

        }

        patternAreaRect = newPatternAreaRect;

    }

    cv::waitKey(0);
}