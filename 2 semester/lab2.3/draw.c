#include <windows.h>
#include "Vertex.c"

#define VERTICES_COUNT 11
#define VERTEX_RADIUS 32
#define VERTEX_DIAMETER (2 * VERTEX_RADIUS)
#define TEXT_MARGIN 5

void drawVertex(HPEN vertexPen, Vertex *vertex, HDC hdc)
{
	// малювання вершини
	SelectObject(hdc, vertexPen);
	Ellipse(hdc, vertex->x - VERTEX_RADIUS, vertex->y - VERTEX_RADIUS, vertex->x + VERTEX_RADIUS, vertex->y + VERTEX_RADIUS);
	char vertexName[3];
	sprintf(vertexName, "%d", vertex->num);
	// написання номеру вершини
	if (vertex->num <= 9)
	{
		TextOut(hdc, vertex->x - TEXT_MARGIN, vertex->y - VERTEX_RADIUS / 4, vertexName, 1);
	}
	else
	{
		TextOut(hdc, vertex->x - TEXT_MARGIN - 4, vertex->y - VERTEX_RADIUS / 4, vertexName, 2);
	}
}

void drawArrow(HPEN arrowPen, double rotateAngle, double arrowX, double arrowY, double arrowLength, HDC hdc)
{
	double lx, ly, rx, ry;
	lx = arrowX + arrowLength * cos(rotateAngle + 0.3);
	rx = arrowX + arrowLength * cos(rotateAngle - 0.3);
	ly = arrowY + arrowLength * sin(rotateAngle + 0.3);
	ry = arrowY + arrowLength * sin(rotateAngle - 0.3);
	MoveToEx(hdc, lx, ly, NULL);
	LineTo(hdc, arrowX, arrowY);
	LineTo(hdc, rx, ry);
}

void drawEdge(HPEN edgePen, double startX, double startY, double endX, double endY, HDC hdc)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

void drawArrowedEdge(HPEN edgePen, double startX, double startY, double endX, double endY, HDC hdc)
{
	double rotateAngle = atan2(startY - endY, startX - endX);
	double arrowX = endX + VERTEX_RADIUS * cos(rotateAngle);
	double arrowY = endY + VERTEX_RADIUS * sin(rotateAngle);

	drawEdge(edgePen, startX, startY, endX, endY, hdc);
	drawArrow(edgePen, rotateAngle, arrowX, arrowY, VERTEX_RADIUS, hdc);
}

void drawReflectEdge(HPEN edgePen, Vertex *vertex, HDC hdc)
{
	if (vertex->num <= (VERTICES_COUNT / 4)) // 3 чверть
	{
		Ellipse(hdc, vertex->x - VERTEX_DIAMETER, vertex->y + VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 2) // 2 чверть
	{
		Ellipse(hdc, vertex->x - VERTEX_DIAMETER, vertex->y - VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 3) // 1 чверть
	{
		Ellipse(hdc, vertex->x + VERTEX_DIAMETER, vertex->y - VERTEX_DIAMETER, vertex->x, vertex->y);
	}
	else // 4 чверть
	{
		Ellipse(hdc, vertex->x + VERTEX_DIAMETER, vertex->y + VERTEX_DIAMETER, vertex->x, vertex->y);
	}
}

void drawArrowedReflectEdge(HPEN edgePen, Vertex *vertex, HDC hdc)
{
	drawReflectEdge(edgePen, vertex, hdc);

	double rotateAngle;
	double arrowX = vertex->x;
	double arrowY = vertex->y;

	if (vertex->num <= (VERTICES_COUNT / 4)) // 3 чверть
	{
		rotateAngle = toRadians(165.0);
		arrowX -= 32;
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 2) // 2 чверть
	{
		rotateAngle = toRadians(195.0);
		arrowX -= 32;
	}
	else if (vertex->num <= (VERTICES_COUNT / 4) * 3) // 1 чверть
	{
		rotateAngle = toRadians(345.0);
		arrowX += 32;
	}
	else // 4 чверть
	{
		rotateAngle = toRadians(15.0);
		arrowX += 32;
	}
	drawArrow(edgePen, rotateAngle, arrowX, arrowY, VERTEX_RADIUS / 2, hdc);
}

void drawArrowedCurveEdge(HPEN edgePen, double startX, double startY, double endX, double endY, HDC hdc)
{
	double centerX = (startX + endX) / 2 - 32;
	double centerY = (startY + endY) / 2 - 32;
	drawEdge(edgePen, startX, startY, centerX, centerY, hdc);
	drawArrowedEdge(edgePen, centerX, centerY, endX, endY, hdc);
}
