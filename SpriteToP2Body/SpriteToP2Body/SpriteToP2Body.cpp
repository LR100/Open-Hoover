// SpriteToP2Body.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Path.h"

using namespace std;



cv::Mat search_for_format(std::string name)
{
	cv::Mat image;
	std::string filename;
	size_t i = 0;
	std::vector<std::string> formats;

	formats.push_back("bmp");
	formats.push_back("jpg");
	formats.push_back("jpeg");
	formats.push_back("png");

	for (; image.empty() && i < formats.size(); i += 1)                              // Check for invalid input
	{
		filename = (name + "." + formats.at(i));
		std::cout << "Search for (" << filename << ")" << std::endl;
		image = cv::imread(filename, cv::IMREAD_COLOR);   // Read the file
	}

	return (image);
}


/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}


/**
 * Helper function to display text in the center of a contour
 */
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.1;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), cv::FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}

#include <sstream>




void createObject(cv::Mat image, size_t index)
{
	std::stringstream ss;

	ss.str("");
	ss << "origin" << index;
	cv::String winOrigin(ss.str());
	ss.str("");
	ss << "mode" << index;
	cv::String winMode(ss.str());
	ss.str("");
	ss << "gray" << index;
	cv::String winGray(ss.str());
	cv::Size size(500, 500);

	// Create and resize windows
	cv::namedWindow(winOrigin, cv::WINDOW_NORMAL);// Create a window for display.
	cv::namedWindow(winMode, cv::WINDOW_NORMAL);// Create a window for display.
	cv::namedWindow(winGray, cv::WINDOW_NORMAL);// Create a window for display.

	if (!image.empty())
	{
		// Get Border Values

		uint64_t r, g, b; // Total
		uint64_t count = 0;

		r = 0;
		g = 0;
		b = 0;

		// Top line
		for (size_t x = 0; x < image.cols; x += 1) {
			cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x, 0));
			r += color[0]; g += color[1]; b += color[2];
			count += 1;
		}
		// Bot line
		for (size_t x = 0; x < image.cols; x += 1) {
			cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x, (image.rows - 1)));
			r += color[0]; g += color[1]; b += color[2];
			count += 1;
		}
		// Left Line (one on right
		for (size_t y = 0; y < image.rows; y += 1) {
			cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(0, y));
			r += color[0]; g += color[1]; b += color[2];
			count += 1;
		}
		// Right Line
		for (size_t y = 0; y < image.rows; y += 1) {
			cv::Vec3b color = image.at<cv::Vec3b>(cv::Point((image.cols - 1), y));
			r += color[0]; g += color[1]; b += color[2];
			count += 1;
		}

		r /= count;
		g /= count;
		b /= count;

		std::cout << "Value Of Borders : (" << r << ") (" << g << ") (" << b << ")" << std::endl;

		// Add larger border to original image
		cv::Mat bigger(image.rows + 200, image.cols + 200, image.type(), cv::Scalar(r, g, b));



		image.copyTo(bigger(cv::Rect(100, 100, image.cols, image.rows)));
		image = bigger;
	}
	else {
		std::cerr << "No Sprite Found" << std::endl;
		cv::waitKey(0);
		return;
	}


	int y = (30 * index) + 50;

	cv::moveWindow(winOrigin, 100, y);
	cv::moveWindow(winMode, 600, y);
	cv::moveWindow(winGray, 1100, y);

	cv::resizeWindow(winOrigin, size);
	cv::resizeWindow(winMode, size);
	cv::resizeWindow(winGray, size);

	// Modify ORIGINAL IMAGE //

	


	// di
	imshow(winOrigin, image);                   // Show our image inside it.


		//aperture = 3

		//dst = cv.CreateImage(cv.GetSize(gray), cv.IPL_DEPTH_32F, 1)
		//cv.Laplace(gray, dst, aperture)

		//cv.Convert(dst, gray)

		//thresholded = cv.CloneImage(im)
		//cv.Threshold(im, thresholded, 50, 255, cv.CV_THRESH_BINARY_INV)

		//cv.ShowImage('Laplaced grayscale', gray)




	cv::Mat		gray;
	cv::Point	point;


	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(gray, gray, cv::Size(1, 1), 0);
	cv::threshold(gray, gray, 220, 255, cv::THRESH_BINARY_INV);
	cv::GaussianBlur(gray, gray, cv::Size(3, 3), 0);
	


	ss.str("");
	ss << "grayO" << index;
	cv::String grayO(ss.str());
	cv::namedWindow(grayO, cv::WINDOW_NORMAL);// Create a window for display.
	cv::resizeWindow(grayO, size);
	cv::moveWindow(grayO, 1100, y + 10);
	cv::imshow(grayO, gray);

	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_8S;
	
	// Use Canny instead of threshold to catch squares with gradient shading

	ss.str("");
	ss << "canny" << index;
	cv::String lap(ss.str());
	//cv::Canny(gray, gray, 0, 200, 3);
	// cv::Canny(gray, gray, 0.2, 0.95);
	//cv::GaussianBlur(gray, gray, cv::Size(3, 3), 0);
	// cv::Laplacian(gray, gray, ddepth, kernel_size, scale, delta, cv::BORDER_DEFAULT);
	
	
	cv::namedWindow(lap, cv::WINDOW_NORMAL);// Create a window for display.
	cv::resizeWindow(lap, size);
	cv::moveWindow(lap, 1100, y + 30);
	cv::imshow(lap, gray);
	

	// Find contours
	std::vector<std::vector<cv::Point> > contours;

	cv::findContours(gray.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	std::vector<cv::Point> approx;
	cv::Mat dst = image.clone();


	// Clear Dest
	dst = cv::Scalar(255, 255, 255);

	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		double perimeter = cv::arcLength(cv::Mat(contours[i]), true);
		cv::approxPolyDP(cv::Mat(contours[i]), approx, perimeter * 0.01, true);


		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(contours[i])) < 100)
		{
			// std::cerr << "WARNING: Contour is too small" << std::endl;
		}
		else
		{

			if (!isContourConvex(contours[i])) {
				std::cerr << "WARNING: Contour is not Convex" << std::endl;
			}
			std::cout << "Contours[" << i << "] is Valid" << std::endl;

			// Draw Center
			cv::Moments m = moments(contours[i]);

			if (m.m00 != 0) { // If shape have a mass center

				point.x = (m.m10 / m.m00);
				point.y = (m.m01 / m.m00);

				std::cout << "With Center at (" << point.x << ") (" << point.y << ")" << std::endl;
				circle(dst, point, 1, (0, 255, 0));
				// Draw Contours
				int thickness = (std::max(image.cols, image.rows) / 200) + 1;

				
				cv::drawContours(dst, contours, i, (255, 255, 0), thickness);

				if (approx.size() == 3)
				{
					setLabel(dst, "TRI", contours[i]);    // Triangles
				}
				else if (approx.size() >= 4 && approx.size() <= 6)
				{
					// Number of vertices of polygonal curve
					int vtc = approx.size();

					// Get the cosines of all corners
					std::vector<double> cos;
					for (int j = 2; j < vtc + 1; j++)
						cos.push_back(angle(approx[j % vtc], approx[j - 2], approx[j - 1]));

					// Sort ascending the cosine values
					std::sort(cos.begin(), cos.end());

					// Get the lowest and the highest cosine
					double mincos = cos.front();
					double maxcos = cos.back();

					// Use the degrees obtained above and the number of vertices
					// to determine the shape of the contour
					if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3) {
						//  setLabel(dst, "RECT", contours[i]);
					}
					else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27) {
						// setLabel(dst, "PENTA", contours[i]);
					}
					else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45) {
						// setLabel(dst, "HEXA", contours[i]);
					}
				}
				else
				{

					// Detect and label circles
					double area = cv::contourArea(contours[i]);
					cv::Rect r = cv::boundingRect(contours[i]);
					int radius = r.width / 2;

					if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
						std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
					{
						// setLabel(dst, "CIR", contours[i]);
						// circle(dst, point, radius, (255, 0, 0));
						/// Create Circle of radius at position 

					}

				}

			}
			else {
				std::cout << "With no mass Center" << std::endl;
			}

		}

	}

	cv::imshow(winMode, dst);
	cv::imshow(winGray, gray);
}



void loadSpriteFolder()
{
	std::vector<cv::String> filenames; // notice here that we are using the Opencv's embedded "String" class
	cv::String folder = (Path::GetExePath() + "/sprites/"); // again we are using the Opencv's embedded "String" class

	std::cout << "Path Exe (" << Path::GetExePath() << ")" << std::endl;
	cv::glob(folder, filenames); // new function that does the job ;-)

	std::cout << "Files in (" << folder << ") Folder (" << filenames.size() << ")" << std::endl;

	for (size_t i = 0; i < filenames.size(); ++i)
	{
		std::cout << "File : (" << filenames.at(i) << ")" << std::endl;
		cv::Mat image = imread(filenames.at(i), cv::IMREAD_UNCHANGED);   // Read the file
		// Split channels of the png files
		std::vector<cv::Mat> pngChannels(4);
		cv::Mat image_output;

		cv::split(image, pngChannels);
		if (pngChannels.size() == 4) {

			// Set to 255(white) the RGB channels where the Alpha channel(mask) is 0(transparency)
			pngChannels[0].setTo(cv::Scalar(255), pngChannels[3] == 0);
			pngChannels[1].setTo(cv::Scalar(255), pngChannels[3] == 0);
			pngChannels[2].setTo(cv::Scalar(255), pngChannels[3] == 0);


			// Merge again the channels

			cv::merge(pngChannels, image_output);
			cv::cvtColor(image_output, image_output, cv::COLOR_RGBA2RGB);
		}
		else {
			image_output = image;
		}

		createObject(image_output, i);

		/* do whatever you want with your images here */
	}
}


int main()
{
	loadSpriteFolder();
	cv::waitKey(0);
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
