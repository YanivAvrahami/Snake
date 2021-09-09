// Name: Yaniv Avrahami
// ID: 302374921

// This program is a game called snake. the player need to direct 
// the snake with the keys a,s,d,w to avoid colliding with the 
// boundaries and himself, while the snake is extending.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct _Point {
	int x;
	int y;
}Point;

typedef struct _Node {
	Point position;
	struct _Node* next;
}Node;

typedef struct _Direction {
	int x; // 1 or 0 or -1
	int y; // 1 or 0 or -1
}Direction;

typedef struct _Snake {
	Node* head;
	Node* tail;
	int length;
	Direction direction;
	float speed;
	float maxSpeed;
	int stepCount;
	int stepsToExtend;
	char skin;
}Snake;

typedef struct _Rect {
	Point point1;
	Point point2;
}Rect;

const Rect BOUNDARIES = { {1, 1}, {75, 25} };
const char SNAKE_SKIN = '@';
const char WALL_SKIN = '#';
const Direction INITIAL_DIRECTION = { 1, 0 };
const float INITIAL_SPEED = 2.5f;
const float INITIAL_MAX_SPEED = 3.0f;
const int INITIAL_STEPS_TO_EXTEND = 4;
const Direction UP =   { 0, -1 };
const Direction DOWN = { 0, 1 };
const Direction LEFT = { -1, 0 };
const Direction RIGHT = { 1, 0 };
const char* GAME_OVER_MESSAGE = "GAME OVER";

// Turn escape codes ON.
void init(void);

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y);

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs);


// Prints a char in a specific point.
void printChar(Point point, char ch);

// Prints a horizontal line of chars, between two points.
void printCharHLine(Point point1, Point point2, char ch);

// Prints a vertical line of chars, between two points.
void printCharVLine(Point point1, Point point2, char ch);

// Prints a rectangle of chars.
void printCharRect(Rect rect, char ch);

// Prints a text at a specific point.
void printText(Point point, char* str);


// Creates a point struct.
Point createPoint(int x, int y);

// Creates a rectangle struct.
Rect createRect(Point point1, Point point2);

// Tells if a point is inside a rectangle or not.
bool isInsideRect(Point point, Rect rect);

// Tells if a two points are equal or not.
bool isEqualPoints(Point point1, Point point2);

// Gives the middle point of a rectangle.
Point rectMidPoint(Rect rect);


// Creates a node.
Node* createNode(Point position, Node* next);

// Creates a snake.
Snake* createSnake(Point initialPosition, Direction initialDirection, float initialSpeed, float initialMaxSpeed, int initialStepsToExtend, char skin);

// Adds a node to head of the snake and prints it.
void addNodeToHeadAndPrint(Snake* snake, Point position);

// Adds a node to tail of the snake and prints it.
void addNodeToTailAndPrint(Snake* snake, Point position);

// Removes a node from snake tail and prints empty space.
void removeNodeFromTailAndPrint(Snake* snake);

// Moves the cursor below the rectangluar boundaries.
void moveCursorBelowBoundaries();

// Moves one step in the snake direction.
void moveOneStep(Snake* snake);

// Moves one step in the snake direction and extends the snake.
void moveOneStepAndExtendTail(Snake* snake);

// Gets and process the input of the user.
void ProcessInput(Snake* snake);

// Frees the list of the snake.
void freeSnakeList(Snake* snake);

// Checks if the snake has collided with boundaries.
bool isBoundariesCollisionDetected(Snake* snake, Rect boundaries);

// Checks if the snake has collided with himself.
bool isBodyCollisionDetected(Snake* snake);

// Checks if the snake has collided with himself or boundaries.
bool isCollisionDetected(Snake* snake, Rect boundaries);

// Prints game over screen.
void gameOver(Snake* snake);

// Increases the speed of the snake by num
void increaseSpeed(Snake* snake, float num);


int main() {
	bool game_over = false;

	init();
	printCharRect(BOUNDARIES, WALL_SKIN);
	Snake* snake = createSnake(rectMidPoint(BOUNDARIES), INITIAL_DIRECTION, INITIAL_SPEED, INITIAL_MAX_SPEED, INITIAL_STEPS_TO_EXTEND, SNAKE_SKIN);

	while (game_over == false)
	{



		sleep(1 / snake->speed);

		ProcessInput(snake);

		if (snake->stepCount % snake->stepsToExtend == 0)
		{
			increaseSpeed(snake, 0.03);
			moveOneStepAndExtendTail(snake);
		}
		else
		{
			moveOneStep(snake);
		}

		if (isCollisionDetected(snake, BOUNDARIES))
		{
			game_over = true;
		}
	}

	gameOver(snake);
}


void init(void) {
	system("cls");
}

void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

void sleep(float secs) {
	clock_t clocks_start = clock();

	while (clock() - clocks_start < secs * CLOCKS_PER_SEC)
		;
}


void printChar(Point point, char ch)
{
	gotoxy(point.y, point.x);
	printf("%c", ch);
}

void printCharHLine(Point point1, Point point2, char ch)
{
	int i;
	Point currentPoint;
	currentPoint.y = point1.y;
	for (i = point1.x; i <= point2.x; i++)
	{
		currentPoint.x = i;
		printChar(currentPoint, ch);
	}
}

void printCharVLine(Point point1, Point point2, char ch)
{
	int i;
	Point currentPoint;
	currentPoint.x = point1.x;
	for (i = point1.y; i <= point2.y; i++)
	{
		currentPoint.y = i;
		printChar(currentPoint, ch);
	}
}

void printCharRect(Rect rect, char ch)
{
	Point topLeft = rect.point1;
	Point topRight = createPoint(rect.point2.x, rect.point1.y);
	Point bottomLeft = createPoint(rect.point1.x, rect.point2.y);
	Point bottomRight = rect.point2;

	printCharHLine(topLeft, topRight, ch);
	printCharHLine(bottomLeft, bottomRight, ch);
	printCharVLine(topLeft, bottomLeft, ch);
	printCharVLine(topRight, bottomRight, ch);
}

void printText(Point point, char* str)
{
	int strLength = strlen(str);
	gotoxy(point.y, point.x - strLength / 2);
	printf("%s", str);
}


Point createPoint(int x, int y)
{
	Point newPoint;
	newPoint.x = x;
	newPoint.y = y;
	return newPoint;
}

Rect createRect(Point point1, Point point2)
{
	Rect newRect;
	newRect.point1 = point1;
	newRect.point2 = point2;
	return newRect;
}

bool isInsideRect(Point point, Rect rect)
{
	return ((point.x > rect.point1.x + 1) && (point.x < rect.point2.x - 1)) && ((point.y > rect.point1.y + 1) && (point.y < rect.point2.y - 1));
}

bool isEqualPoints(Point point1, Point point2)
{
	return (point1.x == point2.x) && (point1.y == point2.y);
}

Point rectMidPoint(Rect rect)
{
	return createPoint(rect.point2.x / 2, rect.point2.y / 2);
}

Point rectRandomPoint(Rect rect)
{
	int xLeft = rect.point1.x + 1;
	int xRight = rect.point2.x - 1;
	int xDiff = xRight - xLeft;

	int yTop = rect.point1.y + 1;
	int yBottom = rect.point2.y - 1;
	int yDiff = yBottom - yTop;

	srand(time(NULL));
	return createPoint(rand() % xDiff + xLeft, rand() % yDiff + yTop);
}


Node* createNode(Point position, Node* next) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL)
	{
		printf("Unable to allocate memory");
		exit(1);
	}
	newNode->position = position;
	newNode->next = next;
	return newNode;
}

void moveCursorBelowBoundaries()
{
	gotoxy(BOUNDARIES.point2.y + 1, 0);
}

void addNodeToHeadAndPrint(Snake* snake, Point position) {
	Node* newNode = createNode(position, NULL);
	if (snake->head == NULL)
		snake->head = snake->tail = newNode;
	else {
		newNode->next = snake->head;
		snake->head = newNode;
	}

	printChar(position, snake->skin);

	moveCursorBelowBoundaries();
}

void addNodeToTailAndPrint(Snake* snake, Point position) {
	Node* newNode = createNode(position, NULL);
	if (snake->head == NULL)
		snake->head = snake->tail = newNode;
	else {
		snake->tail->next = newNode;
		snake->tail = newNode;
	}

	printChar(position, snake->skin);

	moveCursorBelowBoundaries();
}

void removeNodeFromTailAndPrint(Snake* snake) {

	if (snake->head == NULL) {
		return;
	}
	else if (snake->head->next == NULL)
	{
		snake->head = NULL;
		snake->tail = NULL;
	}
	else
	{
		Node* currentNode = snake->head;
		while (currentNode->next->next != NULL)
		{
			currentNode = currentNode->next;
		}
		printChar(snake->tail->position, ' ');
		moveCursorBelowBoundaries();
		snake->tail = currentNode;
		free(snake->tail->next);
		snake->tail->next = NULL;
	}

}

Snake* createSnake(Point initialPosition, Direction initialDirection, float initialSpeed, float initialMaxSpeed, int initialStepsToExtend, char skin)
{
	Snake* snake = (Snake*)malloc(sizeof(Snake));
	if (snake == NULL)
	{
		printf("Unable to allocate memory");
		exit(1);
	}
	snake->head = NULL;
	snake->tail = NULL;
	snake->direction = initialDirection;
	snake->speed = initialSpeed;
	snake->maxSpeed = initialMaxSpeed;
	snake->stepCount = 1;
	snake->stepsToExtend = initialStepsToExtend;
	snake->skin = skin;
	snake->length = 1;

	addNodeToHeadAndPrint(snake, initialPosition);
	moveCursorBelowBoundaries();

	return snake;
}

void moveOneStep(Snake* snake)
{
	Point newHeadPosition;
	newHeadPosition.x = snake->head->position.x + snake->direction.x;
	newHeadPosition.y = snake->head->position.y + snake->direction.y;

	addNodeToHeadAndPrint(snake, newHeadPosition);
	removeNodeFromTailAndPrint(snake);

	snake->stepCount++;
}

void moveOneStepAndExtendTail(Snake* snake)
{
	Point lastTailPosition = snake->tail->position;
	moveOneStep(snake);
	addNodeToTailAndPrint(snake, lastTailPosition);
	snake->length++;
	moveCursorBelowBoundaries();
}

void ProcessInput(Snake* snake)
{
	char key;
	if (_kbhit())
	{
		key = _getch();
		switch (key)
		{
		case 'a':
			snake->direction = LEFT;
			break;
		case 's':
			snake->direction = DOWN;
			break;
		case 'd':
			snake->direction = RIGHT;
			break;
		case 'w':
			snake->direction = UP;
			break;
		}
	}
}

void freeSnakeList(Snake* snake) {

	Node* currentNode = snake->head;
	Node* next;

	while (currentNode != NULL) {
		next = currentNode->next;
		free(currentNode);
		currentNode = next;
	}

	snake->head = snake->tail = NULL;
}

bool isBoundariesCollisionDetected(Snake* snake, Rect boundaries)
{
	return !isInsideRect(snake->head->position, boundaries);
}

bool isBodyCollisionDetected(Snake* snake)
{
	if (snake->head == NULL || snake->head->next == NULL)
	{
		return false;
	}
	else
	{
		Node* currentNode = snake->head->next;
		while (currentNode != NULL)
		{
			if (isEqualPoints(snake->head->position, currentNode->position))
			{
				return true;
			}
			currentNode = currentNode->next;
		}

		return false;
	}
}

bool isCollisionDetected(Snake* snake, Rect boundaries)
{
	return isBodyCollisionDetected(snake) || isBoundariesCollisionDetected(snake, boundaries);
}

void gameOver(Snake* snake)
{
	freeSnakeList(snake);
	system("cls");
	printCharRect(BOUNDARIES, WALL_SKIN);
	printText(rectMidPoint(BOUNDARIES), GAME_OVER_MESSAGE);
	moveCursorBelowBoundaries();
}

void increaseSpeed(Snake* snake, float num)
{
	if (snake->speed == snake->maxSpeed)
	{
		return;
	}

	if (snake->speed < snake->maxSpeed)
	{
		snake->speed += num;
	}
	else
	{
		snake->speed = snake->maxSpeed;
	}
}