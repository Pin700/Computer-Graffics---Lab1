
#define _USE_MATH_DEFINES

#include"MethodGauss.h"

#include "Render.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>


typedef long double ld;

using namespace std;

class Point {
public:
	double x;
	double y;
	double z;
	Point(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Point(const Point& A) {
		this->x = A.x;
		this->y = A.y;
		this->z = A.z;
	}
	void DrawPoint() {
		glVertex3d(this->x, this->y, this->z);
	}
	~Point() {

	}
	Point operator + (Point A) {
		A.x = A.x + this->x;
		A.y = A.y + this->y;
		A.z = A.z + this->z;
		return A;
	}
};

class Draw {

public:
	
	static void DrawRectangle(Point A, Point B, double height, bool flag = false) {
		if (flag) {
			glBegin(GL_QUADS);
		}
		A.DrawPoint();
		B.DrawPoint();
		B = B + Point(0, 0, height);
		B.DrawPoint();
		A = A + Point(0, 0, height);
		A.DrawPoint();
		if (flag) {
			glEnd();
		}
	}

	static void DrawTriangles(vector<Point> points, bool flag = false) {
		if (flag) {
			glBegin(GL_TRIANGLES);
		}
		for (int i = 0; i < (int)points.size(); i++) {
			points[i].DrawPoint();
		}
		if (flag) {
			glEnd();
		}
	}
	static void DrawTriangles(Point A, Point B, Point C, bool flag = false) {
		if (flag) {
			glBegin(GL_TRIANGLES);
		}
		glVertex3d(A.x, A.y, A.z);
		glVertex3d(B.x, B.y, B.z);
		glVertex3d(C.x, C.y, C.z);
		if (flag) {
			glEnd();
		}
	}
	static void DrawTriangles(Point A, Point B, Point C, double height, bool flag = false) {
		if (flag) {
			glBegin(GL_TRIANGLES);
		}
		glVertex3d(A.x, A.y, height);
		glVertex3d(B.x, B.y, height);
		glVertex3d(C.x, C.y, height);
		if (flag) {
			glEnd();
		}
	}
	static void RandomColor() {
		glColor3d((ld)((rand() % 999) + 1.0) / 1000, (ld)((rand() % 999) + 1.0) / 1000, (ld)((rand() % 999) + 1.0) / 1000);
	}
	static void Color(int i) {
		switch (i) {
		case 1:
			glColor3d(0.7, 0.6, 0.9);
			break;
		case 2:
			glColor3d(0.8, 0.3, 0.6);
			break;
		case 3:
			glColor3d(0.3, 0.8, 0.3);
			break;
		}
	}
private:

	static Point SearchMidpoint(Point A, Point B) {
		Point midpoint(0, 0, 0);
		midpoint.x = (A.x + B.x) / 2;
		midpoint.y = (A.y + B.y) / 2;
		midpoint.z = (A.z + B.z) / 2;
		return midpoint;
	}

	static double SearchDistancePoints(Point A, Point B) {
		double X = B.x - A.x;
		double Y = B.y - A.y;
		double Z = B.z - A.z;
		X = pow(X, 2);
		Y = pow(Y, 2);
		Z = pow(Z, 2);
		double r = sqrt(X + Y + Z);
		return r;
	}
	
	static Point SearchVector(Point A, Point B) {
		return Point(B.x - A.x, B.y - A.y, B.z - A.z);
	}
	
	static double SearchAngleVector(Point VectorA, Point VectorB) {
		double number1 = VectorA.x * VectorB.x + VectorA.y * VectorB.y + VectorA.z * VectorB.z;
		double number2 = sqrt(pow(VectorA.x, 2) + pow(VectorA.y, 2) + pow(VectorA.z, 2));
		double number3 = sqrt(pow(VectorB.x, 2) + pow(VectorB.y, 2) + pow(VectorB.z, 2));
		double angle = acos(number1 / (number2 * number3));
		angle = angle * 180 / M_PI;
		return angle;
	}
public:

	static vector<Point> DrawConvex(Point B, Point C, double z) {
		
		Point midpoint = SearchMidpoint(B, C);
		double r = SearchDistancePoints(B, midpoint);

	
		double x = midpoint.x + r * cos(0);
		double y = midpoint.y + r * sin(0);
		Point O(x, y, 0);

		
		Point VectorB = SearchVector(midpoint, B);
		Point VectorO = SearchVector(midpoint, O);
		Point VectorC = SearchVector(midpoint, C);

		
		double angleOC = SearchAngleVector(VectorO, VectorC);
		double angleOB = SearchAngleVector(VectorO, VectorB);

		vector<Point> pointsBC;
		Point newPoint(0, 0, 0);
		for (double i = 360 - angleOC; i <= 360; i += 0.2) {
			newPoint.x = midpoint.x + r * cos(i * M_PI / 180);
			newPoint.y = midpoint.y + r * sin(i * M_PI / 180);
			pointsBC.push_back(newPoint);
		}
		for (double i = 0; i <= angleOB; i += 0.2) {
			newPoint.x = midpoint.x + r * cos(i * M_PI / 180);
			newPoint.y = midpoint.y + r * sin(i * M_PI / 180);
			pointsBC.push_back(newPoint);
		}

		
		int size = pointsBC.size();
		for (int i = 0; i < size - 1; i++) {
			DrawRectangle(pointsBC[i], pointsBC[i + 1], z);
		}

		return pointsBC;
	}
private:

	static Point SearchIntersectionPoint(Point A, Point B, Point C, Point D) {
		Point IntersectionPoint(0, 0, 0);

		double number1 = (A.x * B.y - A.y * B.x) * (C.x - D.x);
		double number2 = (A.x - B.x) * (C.x * D.y - C.y * D.x);
		double number3 = (A.x - B.x) * (C.y - D.y);
		double number4 = (A.y - B.y) * (C.x - D.x);

		IntersectionPoint.x = (number1 - number2) / (number3 - number4);

		number1 = (A.x * B.y - A.y * B.x) * (C.y - D.y);
		number2 = (A.y - B.y) * (C.x * D.y - C.y * D.x);

		IntersectionPoint.y = (number1 - number2) / (number3 - number4);

		return IntersectionPoint;
	}

	static Point ApexParabola(double a, double b, double c) {
		double x = (-1 * b) / (2 * a);
		double y = pow(x, 2) * a + x * b + c;
		Point O(x, y, 0);
		return O;
	}

public:
	
	static vector<Point> DrawBulge(Point D, Point M, Point C, double z, Point& Apex) {
		double x[] = { C.x, M.x, D.x };
		double y[] = { C.y, M.y, D.y };

		

		double matrix[3][4];

		
		for (int i = 0; i < 3; i++) {
			matrix[i][0] = pow(x[i], 2);
			matrix[i][1] = x[i];
			matrix[i][2] = 1;
			matrix[i][3] = y[i];
		}

		MethodGauss ÑoefficientsParabola(matrix, 3, 4);

		
		Apex = ApexParabola(ÑoefficientsParabola.decision[0], ÑoefficientsParabola.decision[1], ÑoefficientsParabola.decision[2]);

	
		vector<Point> pointsDC;
		Point newPoint(0, 0, 0);
		for (double i = D.x; i <= C.x; i += 0.1) {
			newPoint.x = i;
			newPoint.y = pow(i, 2) * ÑoefficientsParabola.decision[0] + i * ÑoefficientsParabola.decision[1] + ÑoefficientsParabola.decision[2];
			pointsDC.push_back(newPoint);
		}

		
		int size = pointsDC.size();
		for (int i = 0; i < size - 1; i++) {
			DrawRectangle(pointsDC[i], pointsDC[i + 1], z);
			if ((int)pointsDC[i].x == (int)M.x && (int)pointsDC[i].y == (int)M.y) {
				glColor3d(0, 0, 1);
			}
		}

		return pointsDC;
	}
	
	static void DrawTopBottom(Point A, Point B, Point C, Point D, Point E, Point F, Point G, double z) {
		
		Point intersectionPoint1 = SearchIntersectionPoint(A, Point(0, 0, 0), D, E);
		

		vector<vector<Point>> ThreePoint = { {A, G, F }, { F, E, intersectionPoint1 }, { F, intersectionPoint1, A } };
		for (int i = 0; i < (int)ThreePoint.size(); i++) {
			Color(i);
			DrawTriangles(ThreePoint[i]);
			for (int j = 0; j < (int)ThreePoint[i].size(); j++) {
				ThreePoint[i][j].z = z;
			}
			DrawTriangles(ThreePoint[i]);
		}

		
		Point intersectionPoint2 = SearchIntersectionPoint(A, Point(1, 0, 0), C, B);
		glColor3d(0, 1, 0);
		vector<Point> ThreePoint2 = { A, B, intersectionPoint2 };
		DrawTriangles(ThreePoint2);
		for (int i = 0; i < 3; i++) {
			ThreePoint2[i].z = z;
		}
		DrawTriangles(ThreePoint2);

		glColor3d(0, 1, 1);
		vector<Point> ThreePoint3 = { A, E, intersectionPoint2 };
		DrawTriangles(ThreePoint3);
		for (int i = 0; i < 3; i++) {
			ThreePoint3[i].z = z;
		}
		DrawTriangles(ThreePoint3);



		glColor3d(0, 0, 1);
		vector<Point> ThreePoint4 = { C, E, intersectionPoint2 };
		DrawTriangles(ThreePoint4);
		for (int i = 0; i < 3; i++) {
			ThreePoint4[i].z = z;
		}
		DrawTriangles(ThreePoint4);



	}

	static void DrawTopBottomConvex(Point B, Point C, vector<Point> pointsBC, double z) {
		Point midPoint = SearchMidpoint(B, C);
		int size = pointsBC.size();
		for (int i = 0; i < size - 1; i++) {
			DrawTriangles(midPoint, pointsBC[i], pointsBC[i + 1]);
			DrawTriangles(midPoint, pointsBC[i], pointsBC[i + 1], z);
		}
	}
	static void DrawTopBottomBulge(Point A, Point B, Point C, Point D, Point E, Point ApexParabola, vector<Point> pointsDC, double z) {
		Point O1 = SearchIntersectionPoint(A, Point(0, 0, 0), D, E);
		Point O2 = SearchIntersectionPoint(A, Point(0, 0, 0), B, C);

		int size = pointsDC.size();
		for (int i = 0; i < size - 1; i++) {
			if (pointsDC[i + 1].x < ApexParabola.x) {
				DrawTriangles(O1, pointsDC[i], pointsDC[i + 1]);
				DrawTriangles(O1, pointsDC[i], pointsDC[i + 1], z);
			}
			else {
				DrawTriangles(A, pointsDC[i], pointsDC[i + 1]);
				DrawTriangles(A, pointsDC[i], pointsDC[i + 1], z);
			}
		}

		glColor3d(0.8, 0.1, 0.4);
		DrawTriangles(A, C, O2);
		DrawTriangles(A, C, O2, z);
		
		glColor3d(0.5, 0.2, 0.6);
		DrawTriangles(A, ApexParabola, O1);
		DrawTriangles(A, ApexParabola, O1, z);
	}
};

void Render(double delta_time)
{
	Point A(0, 0, 0), B(3, 2, 0), C(6, -2, 0), D(-9, -4, 0), E(0, -5, 0), F(-5, 2, 0), G(0, 5, 0), M(-2.5, -0.5, 0);
	
	double z = 2;

	glPushMatrix();
	glBegin(GL_QUADS);
	
	glColor3d(0, 0, 0);
	Draw::DrawRectangle(A, B, z);
	
	glColor3d(1, 1, 0);
	Draw::DrawRectangle(C, E, z);
	

	
	glColor3d(1, 0, 1);
	Draw::DrawRectangle(F, G, z);
	
	glColor3d(0, 0, 0);
	Draw::DrawRectangle(G, A, z);
	
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);

	//âûïóêëîñòü
	glColor3d(0.4, 0, 0);

	vector<Point> pointsBC = Draw::DrawConvex(B, C, z);

	glColor3d(1, 0, 1);

	Point ApexParabola(0, 0, 0);
	vector<Point> pointsDC = Draw::DrawBulge(F, M, E, z, ApexParabola);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_TRIANGLES);
	
	glColor3d(1, 0, 0);
	Draw::DrawTopBottom(A, B, C, D, E, F, G, z);

	glColor3d(0.8, 0.8, 0.2);
	Draw::DrawTopBottomConvex(B, C, pointsBC, z);

	glColor3d(0, 0, 0);
	Draw::DrawTopBottomBulge(A, B, C, D, E, ApexParabola, pointsDC, z);

	glEnd();
	glPopMatrix();

}


