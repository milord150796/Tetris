#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;
const int M = 20;//the height of the playing field
const int N = 10 ;// the width of the playing field

int field[M][N]={0};//playing field

struct Point //point with target coordinates 
{
	int x,y;
}a[4],b[4];//two auxiliary arrays

int figures [7][4]= //array of tetris figures
{
	1,3,5,7, // I
	2,4,5,7, // S
	3,5,4,6, // Z
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

//Compliance with the boundaries of the playing field and the occlusion of the cells of the playing field
bool check ()
{
	for (int i=0;i<4;i++)
		if (a[i].x<0||a[i].x>=N||a[i].y>=M)
			return false;
	else if(field[a[i].y][a[i].x])
			return false;
	return true;
};
 
int main()
{
	srand(time(0)); 
	//creating a window with a resolution of 320 by 480 pixels, which will be called Tetris
	RenderWindow window(VideoMode(320, 480), "Tetris");
	Texture t1,t2,t3;
	t1.loadFromFile("D:/game/images/tiles.png");//square
	t2.loadFromFile("D:/game/images/background.png");//backdrop
	t3.loadFromFile("D:/game/images/frame.png");//framing
	//creating and loading textures
	Sprite s(t1),background (t2), frame (t3);//square,backdrop and framing
	int dx = 0;//variable for horizontal displacement of shapes
	bool rotate=false;//variable for rotation
	int colorNum = 1;//shape color number
	bool beginGame = true;//game start checkbox
	int n = rand ()%7;//set a random type of figures (one from the scheme)
	float timer =0;//timer counter
	float delay =0.3;//period of time between moving figures down (in seconds)
	Clock clock;//hours (time elapsed since the start of the game)
	
	//main game loop while window is open
	while (window.isOpen())
	{	//we get the time elapsed since the beginning of the countdown and convert it into seconds
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer +=time;
		//If the first figure on the field
		if (beginGame)
		{	
			beginGame = false;//It's not the start of the game anymore
			n=rand()%7;//random location
			for(int i=0;i<4;i++)
				{	
					a[i].x=figures[n][i]%2;
					a[i].y=figures[n][i]/2;
				}
		}
		//handle events in a loop
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) //the user clicked on the cross and wants to close
				window.close();//Yes please
				//Was the key pressed on laptops?
			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Up)
					rotate=true;//if the button is up then rotated
				else if (e.key.code == Keyboard::Left)
					dx = -1;//if the button is to the left, then it moves to the left
				else if (e.key.code == Keyboard::Right)
					dx=1;//if the button is to the right, it moves to the right
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))	
			delay = 0.05;//if the button is down, then we accelerate the fall of the figures
		//	shifts each square of the figure in the desired direction horizontally
		for(int i=0;i<4;i++)
		{	
			b[i]=a[i];//remember the initial figure
			a[i].x+=dx;//displace
		}
		if (!check())//if boundaries are not respected
			for (int i=0;i<4;i++)
				a[i]=b[i];//then return back
		//rotation	
		if (rotate) 
		{
			Point p=a[1]; //center of rotation
			for (int i=0;i<4;i++)
			{
				int x =a[i].y-p.y;
				int y =a[i].x-p.x;
				a[i].x=p.x-x;
				a[i].y=p.y+y;
			}
			if (!check())
				for (int i=0;i<4;i++)
					a[i]=b[i];
		}
		//manipulation with time (falling figure)
		if (timer > delay)//if passed within the period, then updates the window
		{
				for (int i=0;i<4;i++) //move down one position
				{
					b[i]=a[i];
					a[i].y+=1;
				}
				if(!check())
				{
					for (int i=0;i<4;i++) //close the occupied cells with the desired color
						field [b[i].y][b[i].x]=colorNum;
					colorNum =1+rand()%7;//change the color of the shape randomly
					int n = rand()%7;//change shape type randomly
					for(int i=0;i<4;i++)
					{
						a[i].x=figures[n][i]%2;
						a[i].y=figures[n][i]/2;
						
					}
				}
				timer = 0 ;//reset the timer counter
		}//Returns the previous values of gameplay parameters
		dx=0;
		rotate =false ;
		delay = 0.3;
		//Annihilation of filled lines
		int k=M-1;//coordinates of the bottom line
		for (int i=M-1;i>0;i--)//check counter from bottom to top
		{
			int count =0;//Counter painting squares
			for (int j=0;j<N;j++)
			{
				if (field[i][j])//if the box is not empty
					count++;//then
				field[k][j]=field[i][j];//replace the current line
			}
			if (count<N)k--;
		}
		//Bringing beauty
		window.draw(background);//put a picture in the background
		for (int i=0;i<M;i++)
			for (int j=0;j<N;j++)
			{
				if (field[i][j]==0)	continue;
				s.setTextureRect(IntRect(field[i][j]*18,0,18,18));//rose field
				s.setPosition(j*18,i*18);//set position for each cell
				s.move(28,31);//shift the origin of the field according to the texture of the background
				window.draw(s);//rendering
			}
		for(int i=0;i<4;i++)//do the same for individual shapes
		{
			s.setTextureRect(IntRect(colorNum*18,0,18,18));
			s.setPosition(a[i].x*18,a[i].y*18);
			s.move(28,31);
			window.draw(s);
		}
		window.draw(frame);//put frames
		window.display();//display window
	}
 
	return 0;
}
