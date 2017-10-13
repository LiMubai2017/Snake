#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include"music.h"
void select(void);
void welcome(void);
void pos(int ,int);
void CreatMap(void);
void control(void);
void print(void);
void InitFood(void);
void InitSnake(void);
void InitSnake2(void);
void InitGrass(void);
void InitBrick(void);
void move(void);
void move2(void);
void rank(void);
void AiControl(void); 
void end(void);
void check(void);
void BrickFalling(char);
void BrickRolling(void);
void CheckBrick(void);
void EndPro(int);
void HideCursor();
extern void music(void);
extern void endmusic(void);

const int col[5]={0,40,80,80,80},line[5]={0,20,40,40,40},GrassNumber=10,endcount=10000;
int SleepTime[5]={0,100,20,80,90};
int map[40][80],level=0,score=-10,length,foodcount=0,stepcount=0,GlassFlag=0,SnakeColor=15,NewColor=15,BrickFlag;
//map数组 0为无 1为有边界 2为有食物 3为炸弹 4为毒草 5为1号蛇身  6为砖头  7为停着的砖头 
char direction,direction2;
char update[40][80];
struct Brick{
	int x, y;
}brick[5];
struct Snake{
	int x, y;
	struct Snake *next;
  }*head1,*head2;
struct Food{
	int x,y;
	struct Food *last;
}food;

int main()
{
	welcome();
        CreatMap();
	InitSnake();
	if(level==4) InitSnake2();
	InitFood();
	//AiControl();
	control();
	endmusic();
}
void pos(int x,int y)
{
	COORD c={y,x};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
void CreatMap(void)
{
	int i,BarrierNum=0,BoomNum;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	system("cls");
	if(level==3||level==2||level==4)
      system("mode con cols=80 lines=48");
	if(level==2){
	  getchar();
	  pos(line[level]/2,col[level]/2-14);
	  printf("Enter the number of barriers[0,60]");
	  pos(line[level]/2+1,col[level]/2-4);
	  scanf("%d",&BarrierNum);
	  while(BarrierNum>60||BarrierNum<0){
	  	pos(line[level]/2+2,col[level]/2-4);
	  	printf("     ");
	  	pos(line[level]/2+1,col[level]/2-8);
	  	printf("Enter again(0<=num<=60)");
	  	pos(line[level]/2+2,col[level]/2-4);
	  	scanf("%d",&BarrierNum);
	  	
	  }
	  system("cls");
	  
	  pos(line[level]/2,col[level]/2-14);
	  printf("Enter the number of booms[0,60]");
	  pos(line[level]/2+1,col[level]/2-4);
	  scanf("%d",&BoomNum);
	  while(BoomNum>60||BoomNum<0){
	  	pos(line[level]/2+2,col[level]/2-4);
	  	printf("     ");
	  	pos(line[level]/2+1,col[level]/2-8);
	  	printf("Enter again(0<=num<=60)");
	  	pos(line[level]/2+2,col[level]/2-4);
	  	scanf("%d",&BoomNum);
	  }
	  system("cls");
      }
      
	pos(0,0);
	for(i=0;i<=col[level]-2;i+=2) {printf("■");map[0][i]=1;}
	for(i=1;i<line[level]-1;i++)
	  {
	  	pos(i,0);
	  	printf("■");
	  	pos(i,col[level]-2);
	  	printf("■");
	  	map[i][0]=map[i][col[level]-2]=1;
	  }
	printf("\n");
	for(i=0;i<=col[level]-2;i+=2) {printf("■");map[line[level]-1][i]=1;}
	pos(line[level]+1,col[level]/2-4);
	printf("Score:");
	pos(line[level]+1,col[level]/2+2);
	printf("0");
	pos(line[level]+2,col[level]/2-8);
	printf("↑↓←→移动 空格暂停");  
	if(level==2){
		pos(line[level]+3,col[level]/2-14);
	    printf("彩色方块为食物，!!为地雷,*为毒草");
		int x,y;
		for(i=1;i<=BarrierNum;i++){
		  x=1+rand()%(line[level]-3);
		  y=2+rand()%(col[level]-6);
		  if(y%2!=0) y++;
		  map[x][y]=1;
		  pos(x,y);
		  printf("■");
		}
		for(i=1;i<=BoomNum;i++){
		  x=1+rand()%(line[level]-3);
		  y=2+rand()%(col[level]-6);
		  if(y%2!=0) y++;
		  map[x][y]=3;
		  pos(x,y);
		  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
		  printf("!!");
	    }
	}
	if(level==3){
	  pos(line[level]+3,col[level]/2-16);
	  printf("彩色方块为食物，a左移,d右移，w变换");
	  InitBrick();
    }
    if(level==4){
      pos(line[level]+3,col[level]/2-16);
	  printf("击败对方，取得胜利");
	} 
}
void rank()
{
	FILE *fin,*fout;
	int max,i,Score[5];
	char name[6][15];
	
	memset(Score,0,sizeof(Score));
    i=0;
    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    pos(line[level]/2-3,col[level]/2-4);
    printf("Game over");
	pos(line[level]/2-2,col[level]/2-10);
	printf("Please enter your name");
	pos(line[level]/2-1,col[level]/2-4);
	fgets(name[5],14,stdin);
	switch(level){
	  case 1:fin=fopen("ranking1.in","r");break;
	  case 2:fin=fopen("ranking2.in","r");break;
	  case 3:fin=fopen("ranking3.in","r");break;
    }
	for(i=0;i<=4;i++)
	  fscanf(fin,"%s %d",name[i],&Score[i]);
	if(fclose(fin)==EOF) exit(EXIT_FAILURE);
	for(i=0;i<=4;i++)
	  if(score>Score[i]){
	  	int j;
	  	for(j=4;j>i;j--){
	  	  Score[j]=Score[j-1];
	  	  strcpy(name[j],name[j-1]);
	    }
	    Score[i]=score;
	    int length=strlen(name[5]);
	    for(int z=0;z<=length;z++)
	      if(name[5][z]=='\n')
	        name[5][z]='\0';
	    strcpy(name[i],name[5]);
	    switch(level){
		  case 1:fout=fopen("ranking1.in","w");break;
		  case 2:fout=fopen("ranking2.in","w");break;
		  case 3:fout=fopen("ranking3.in","w");break;
	    }
	    for(i=0;i<=4;i++)
	      fprintf(fout,"%s %d\n",name[i],Score[i]);
	    fclose(fout);
	    break ;
	    }
    system("cls");
    pos(line[level]/2-5,col[level]/2-4);
    printf("%s",name[5]);
	pos(line[level]/2-4,col[level]/2-8);
	printf("Your score: %d",score);
	pos(line[level]/2-2,col[level]/2-4);
	printf("Ranking");
	pos(line[level]/2-1,col[level]/2-8);
	printf("Name and Score");
	for(i=0;i<=4;i++){
		pos(line[level]/2+i,col[level]/2-8);
		printf("%-10s",name[i]);
		pos(line[level]/2+i,col[level]/2+2);
		printf("%d",Score[i]);
	}
	getchar(); 
	exit(EXIT_SUCCESS);
}
void select(void)
{
	int current=9;
	char ch;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),127); 
	pos(9,18);printf("Level 1");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	pos(10,18);printf("Level 2");
	pos(11,18);printf("Level 3"); 
	pos(12,18);printf("Level 4");
	pos(17,12);printf("↑↓选择，回车确定  "); 
	while(1){
		  if(GetAsyncKeyState(VK_UP)&&current>9){
	        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	        pos(current,18);
	        printf("Level %d",current-8);
	        current--;
	        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),127);
	        pos(current,18);
	        printf("Level %d",current-8);
	        Sleep(100);
		    }
		  if(GetAsyncKeyState(VK_DOWN)&&current<12){
		  	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	        pos(current,18);
	        printf("Level %d",current-8);
	        current++;
	        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),127);
	        pos(current,18);
	        printf("Level %d",current-8);
			Sleep(100); 
		    }
		  if(GetAsyncKeyState(VK_RETURN)){
		    break;
	      }
	}
	level=current-8;
}

void InitFood(void)
{
	struct Food current;
	score+=10;
	while((NewColor=1+rand()%14)==10||NewColor==14) continue;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),NewColor);
	pos(line[level]+1,col[level]/2+2);
	printf("%d",score);
	while(map[current.x=2+rand()%(line[level]-2)][current.y=4+rand()%(col[level]-8)]||current.y%2!=0) continue ;
	pos(food.x,food.y);
	printf(" ");
	pos(current.x,current.y);
	printf("■");
	map[food.x][food.y]=1;
	map[current.x][current.y]=2;
	food=current;
}
void InitBrick(void)
{
	BrickFlag=1+rand()%4;
	brick[1].x=1;
	while((brick[1].y=8+rand()%(col[level]-20))%2!=0) continue ;
	switch(BrickFlag){
		case 1:{
			brick[2].x=brick[3].x=brick[4].x=1;
			brick[2].y=brick[1].y-2;
			brick[3].y=brick[1].y+2;
			brick[4].y=brick[1].y+4;
		}break ;//长条 
		case 2:{
			brick[2].x=2;brick[3].x=brick[4].x=1;
			brick[2].y=brick[1].y;
			brick[3].y=brick[1].y+2;
			brick[4].y=brick[1].y+4;
		}break ;//L条 
		case 3:{
		    brick[2].x=brick[3].x=1;brick[4].x=2;
		    brick[4].y=brick[1].y;
			brick[2].y=brick[1].y-2;
			brick[3].y=brick[1].y+2; 
		}break ;//凸 
		case 4:{
			brick[2].x=1;brick[3].x=brick[4].x=2;
			brick[4].y=brick[2].y=brick[1].y+2;
			brick[3].y=brick[1].y;
		}break ;//正方形 
	}
	
}
void InitGrass(void)
{
	int x,y,i,j;
	if(GlassFlag==0){
	  for(i=1;i<=GrassNumber;i++){
	    x=2+rand()%(line[level]-2);
	    y=2+rand()%(col[level]-6);
	    if(y%2!=0) y++;
	    while(map[x][y]!=0)
	      x=2+rand()%(line[level]-2);
	    map[x][y]=4;
        }
        GlassFlag=1;
	}
	else {
		for(i=1;i<=line[level]-2;i++)
		  for(j=2;j<=col[level]-4;j+=2)
		    if(map[i][j]==4){
		    	pos(i,j);
		    	printf(" ");
		    	map[i][j]=0;
		    }
		GlassFlag=0;
		
	}
}
void InitSnake(void)
{
	struct Snake *current;
	length=2;
	direction='D';
	head1=(struct Snake *)malloc(sizeof(struct Snake));
	head1->x=line[level]/2;head1->y=col[level]/2;
	current=head1->next=(struct Snake *)malloc(sizeof(struct Snake)); 
	current->x=line[level]/2-1;current->y=col[level]/2;current->next=NULL;
	current=head1;
	map[line[level]/2][col[level]/2]=map[line[level]/2-1][col[level]/2]=5;
	while(current!=NULL){
	  pos(current->x,current->y);
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
	  printf("■");
	  current=current->next;	
	  }
	food.x=food.y=2;
}
void InitSnake2(void)
{
	struct Snake *current;
	direction2='D';
	head2=(struct Snake *)malloc(sizeof(struct Snake));
	head2->x=line[level]/2;head2->y=col[level]/2+4;
	current=head2->next=(struct Snake *)malloc(sizeof(struct Snake)); 
	current->x=line[level]/2-1;current->y=col[level]/2+4;current->next=NULL;
	current=head2;
	map[line[level]/2][col[level]/2+4]=map[line[level]/2-1][col[level]/2+4]=5;
	while(current!=NULL){
	  pos(current->x,current->y);
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
	  printf("■");
	  current=current->next;	
	  }
}
int check(int a,int b)
{
	int new_x,new_y;
	new_x=head1->x+a;
	new_y=head1->y+b;
	if(map[new_x][new_y]==0||map[new_x][new_y]==2) return 1;
	return 0; 
	
}
void CheckBrick(void)
{
	int i,j,CleanFlag;
	for(i=line[level]-2;i>=1;i--){
      CleanFlag=1;
	  for(j=2;j<=col[level]-4;j+=2)
	    if(map[i][j]!=7) CleanFlag=0;
	  if(CleanFlag) break ;
    }
    if(!CleanFlag) return ;
    score+=10;
	Beep(400,100); 
	int UpdateBrick[40][80];//1为从无到有，2为从有到无 
	memset(UpdateBrick,0,sizeof(UpdateBrick));
	for(i=2;i<=line[level]-2;i++)
	  for(j=2;j<=col[level]-4;j+=2)
	     if(map[i][j]==0&&map[i-1][j]==7) UpdateBrick[i][j]=1;
	     else if(map[i][j]==7&&map[i-1][j]==0) UpdateBrick[i][j]=2;
	for(i=2;i<=line[level]-2;i++)
	  for(j=2;j<=col[level]-4;j+=2)
	    if(UpdateBrick[i][j]==1){
	    	pos(i,j);
	    	printf("■");
	    	map[i][j]=7;
	    }
	    else if(UpdateBrick[i][j]==2){
	    	pos(i,j);
	        printf(" ");
	    	map[i][j]=0;
		}
	CheckBrick();
}
void AiControl(void)
{
	int delta_x,delta_y;
	delta_x=(food.x)-(head1->x);
	delta_y=(food.y)-(head1->y);
	while(1){
	if(level==1) Sleep(SleepTime[1]);
	if(level==2) Sleep(SleepTime[2]);
	if(level==3) Sleep(SleepTime[3]);
	if(GetAsyncKeyState(VK_SPACE))
		while(1){
		Sleep(500);
		if(GetAsyncKeyState(VK_SPACE)) break;
		  }
	if(delta_x>0&&check(1,0))
		direction='D';
	else if(delta_x<0&&check(-1,0))
		direction='U';
	else if(delta_y>0&&check(0,2))
		direction='R';
	else if(delta_y<0&&check(0,-2))
		direction='L';
	else if(check(-1,0))
		direction='U';
	else if(check(1,0))
		direction='D';
	else if(check(0,-2))
		direction='L';
	else if(check(0,2))
		direction='R';
	move();
	delta_x=(food.x)-(head1->x);
	delta_y=(food.y)-(head1->y);
    }
}
void control()
{
	while(1) {
		int flag=0,flag2=0;
		char ch ;
		
		if(GetAsyncKeyState(VK_UP)&&direction!='D'&&direction!='U'&&!flag){
		  direction='U';move();flag=1;	
		}
		if(GetAsyncKeyState(VK_DOWN)&&direction!='D'&&direction!='U'&&!flag){
		  direction='D';move();flag=1;
		}
		if(GetAsyncKeyState(VK_LEFT)&&direction!='L'&&direction!='R'&&!flag){
		  direction='L';move();flag=1;
		}
		if(GetAsyncKeyState(VK_RIGHT)&&direction!='L'&&direction!='R'&&!flag){
		  direction='R';move();flag=1;	
		}
		if(!flag) move();
	    if(level==3&&kbhit()){
		ch=getch();
		switch(ch){
		case 'D':
		case 'd':{ 
		  int BrickFlag=0,i;
		  for(i=1;i<=4;i++)
		    if(brick[i].y>=col[level]-4||map[brick[i].x][brick[i].y+2]==7) BrickFlag=1;
		  if(!BrickFlag) BrickFalling('R');
	    }break ;
	    case 'A':
		case 'a':{ 
		  int BrickFlag=0,i;
		  for(i=1;i<=4;i++)
		    if(brick[i].y<=2||map[brick[i].x][brick[i].y-2]==7) BrickFlag=1;
		  if(!BrickFlag) BrickFalling('L');
	    }break ;
	    case 'W':
		case 'w':{
		  int BrickFlag=0,i;
		  for(i=1;i<=4;i++)
		    if(brick[i].y-4<2) BrickFlag=1;
		    if(brick[i].y+4>col[level]-4) BrickFlag=2;
		  if(BrickFlag){
		  	for(i=1;i<=4;i++){
		  		map[brick[i].x][brick[i].y]=0;
		  		pos(brick[i].x,brick[i].y);
		  		printf(" ");
		  	}
		    if(BrickFlag==1)
		  	  for(i=1;i<=4;i++) brick[i].y+=4; 
            if(BrickFlag==2)
              for(i=1;i<=4;i++) brick[i].y-=4;
            for(i=1;i<=4;i++){
		  		map[brick[i].x][brick[i].y]=6;
		  		pos(brick[i].x,brick[i].y);
		  		printf("■");
		  	}
          }
          BrickRolling();
		  }break ; 
	    }
	    }
	    if(level==4&&kbhit()){
	    ch=' ';
	    ch=getch();
		if((ch=='w'||ch=='W')&&direction2!='D'&&direction2!='U'&&!flag2){
		    direction2='U';move2();flag2=1;	
		}
		if((ch=='s'||ch=='S')&&direction2!='D'&&direction2!='U'&&!flag2){
		   direction2='D';move2();flag2=1;
		}
		if((ch=='a'||ch=='A')&&direction2!='L'&&direction2!='R'&&!flag2){
		   direction2='L';move2();flag2=1;
		}
		if((ch=='d'||ch=='D')&&direction2!='L'&&direction2!='R'&&!flag2){
		  direction2='R';move2();flag2=1;	
		}
		}
	    if(level==4&&!flag2) move2();	
		if(GetAsyncKeyState(VK_SPACE))
		  while(1){
		    Sleep(500);
		    if(GetAsyncKeyState(VK_SPACE)) break;
		  }
		if(level==3) BrickFalling('D');
		if(level==1) Sleep(SleepTime[1]);
	    if(level==2) Sleep(SleepTime[2]);
	    if(level==3) Sleep(SleepTime[3]);
	    if(level==4) Sleep(SleepTime[4]);
	}
	
}
void end(void)
{
	pos(line[level]+1,col[level]/2-4);
	printf("You died");
	getchar();
	rank();
}
void EndPro(int winner)
{
	system("cls");
	pos(line[level]/2,col[level]/2-4);
	if(winner==1){
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
	  printf("红蛇胜");
    }
	else if(winner==2) {
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
	  printf("蓝蛇胜"); 
    }
    else {
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);	
      printf("平局");
    }
	getch();
	exit(EXIT_SUCCESS);
}
void BrickRolling(void)
{
	int i,new_x[5],new_y[5];
	for(i=2;i<=4;i++){
	  new_x[i]=brick[1].x+(brick[i].y-brick[1].y)/2;
	  new_y[i]=brick[1].y-(brick[i].x-brick[1].x)*2;
    }
    for(i=2;i<=4;i++){
      map[brick[i].x][brick[i].y]=0;
      pos(brick[i].x,brick[i].y);
	  printf(" ");
	}
	for(i=2;i<=4;i++){
	  brick[i].x=new_x[i];
	  brick[i].y=new_y[i];
	  map[brick[i].x][brick[i].y]=6;
	  pos(brick[i].x,brick[i].y);
	  printf("■");
	}
}
void BrickFalling(char dir)
{
	int i,StopFlag=0;;
	for(i=1;i<=4;i++){
	  pos(brick[i].x,brick[i].y);
	  printf(" ");
	  map[brick[i].x][brick[i].y]=0;
	}
	for(i=1;i<=4;i++)
	  switch(dir){
	    case 'D':brick[i].x++;break ;
	    case 'L':brick[i].y-=2;break;
	    case 'R':brick[i].y+=2;break; 
	  }
	for(i=1;i<=4;i++){
	  if(map[brick[i].x][brick[i].y]==5) end();
	  if(map[brick[i].x][brick[i].y]==2) {InitFood();score-=10;}
	  if(brick[i].x==line[level]-2||map[brick[i].x+1][brick[i].y]==7)  StopFlag=1;
    }
    for(i=1;i<=4;i++){
	  map[brick[i].x][brick[i].y]=6;
	  pos(brick[i].x,brick[i].y);
	  printf("■");
	}
	if(StopFlag){
	  for(i=1;i<=4;i++)  map[brick[i].x][brick[i].y]=7;
	  CheckBrick(); 
	  InitBrick();
	} 
}
void move(void)
{
	int x,y;
	struct Snake *new_last,*current;
	
	memset(update,0,sizeof(update));//1为从有到无，2为从无到有 
	current=(head1->next);
	new_last=head1;
	while(current->next!=NULL){
	    new_last=current;
	    current=current->next;
	}
	switch(direction){
	  case 'U':{x=head1->x-1;y=head1->y;}break;
	  case 'D':{x=head1->x+1;y=head1->y;}break;	
	  case 'L':{x=head1->x;y=head1->y-2;}break;
	  case 'R':{x=head1->x;y=head1->y+2;}break;
	}
	if(map[x][y]==1||map[x][y]==5||map[x][y]==6) 
	  if(level!=4) end();
	  else EndPro(2);
	if(map[x][y]==2){
		if(level!=4) SnakeColor=NewColor;
		foodcount++;
		music();
		length++;
	    update[x][y]=2;
		map[x][y]=5;
		struct Snake *newone;
		newone=(struct Snake*)malloc(sizeof(struct Snake));	
		newone->x=x;
		newone->y=y;
		newone->next=head1;
		head1=newone;
		InitFood();
	}
	if(map[x][y]==0){
	update[x][y]=2;
	map[x][y]=5;
	new_last->next=NULL;
	update[current->x][current->y]=1;
	map[current->x][current->y]=0;
	current->x=x;
	current->y=y;
	current->next=head1;
	head1=current;
	}
	if(map[x][y]==3||map[x][y]==4){
		foodcount=0;
		if(length<=2) end();
		length--;
		score-=5;
		Beep(880,100);
		new_last->next=NULL;
	    update[current->x][current->y]=1;
	    map[current->x][current->y]=0;
	    free(current);
		map[x][y]=0;	
	}
	print();
} 
void move2(void)
{
	int x,y;
	struct Snake *new_last,*current;
	
	memset(update,0,sizeof(update));//1为从有到无，2为从无到有 
	current=(head2->next);
	new_last=head2;
	while(current->next!=NULL){
	    new_last=current;
	    current=current->next;
	}
	switch(direction2){
	  case 'U':{x=head2->x-1;y=head2->y;}break;
	  case 'D':{x=head2->x+1;y=head2->y;}break;	
	  case 'L':{x=head2->x;y=head2->y-2;}break;
	  case 'R':{x=head2->x;y=head2->y+2;}break;
	}
	if(map[x][y]==1||map[x][y]==5) EndPro(1);
	if(map[x][y]==2){
		music();
	    update[x][y]=4;
		map[x][y]=5;
		struct Snake *newone;
		newone=(struct Snake*)malloc(sizeof(struct Snake));	
		newone->x=x;
		newone->y=y;
		newone->next=head2;
		head2=newone;
		InitFood();
	}
	if(map[x][y]==0){
	update[x][y]=4;
	map[x][y]=5;
	new_last->next=NULL;
	update[current->x][current->y]=3;
	map[current->x][current->y]=0;
	current->x=x;
	current->y=y;
	current->next=head2;
	head2=current;
	}
	print();
} 
void print(void)
{
	int i,j;
	stepcount++;
	if(level==4){
	  pos(line[level]+3,col[level]/2-4);
	  printf("Time left :%d",endcount-stepcount);
	  if(stepcount==endcount){
	    int length1=0,length2=0;
	    struct Snake *current;
	    current=head1;
	    while(current!=NULL){
	    	length1++;
	    	current=current->next;
	    }
	    current=head2;
	    while(current!=NULL){
	    	length2++;
	    	current=current->next;
	    }
	    if(length1>length2) EndPro(1);
	    if(length1<length2) EndPro(2);
	    if(length1==length2) EndPro(0);
	  }
    }  
	if(level==2) 
	  if(stepcount%100==0) InitGrass();
	if(GlassFlag){
		if(stepcount%2==0) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
		else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
		for(i=1;i<=line[level]-2;i++)
		  for(j=2;j<=col[level]-4;j+=2){
		    if(map[i][j]==4){
		    	pos(i,j);
		    	printf("*");
		    }
		}
	}
	if(level==4) SnakeColor=12;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),SnakeColor);
	for(i=1;i<=line[level]-2;i++)
	  for(j=2;j<=col[level]-4;j+=2)
	  if(update[i][j]==2){
	    pos(i,j);
		printf("■");	
		update[i][j]=0;
	  }
	  else if(update[i][j]==1){
	  	pos(i,j);
	  	printf(" ");
	  	update[i][j]=0;
	  }
	if(level==4){
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
	  for(i=1;i<=line[level]-2;i++)
	    for(j=2;j<=col[level]-4;j+=2)
	      if(update[i][j]==4){
	        pos(i,j);
		    printf("■");	
	      }
	      else if(update[i][j]==3){
	            	pos(i,j);
	  	            printf(" ");
	  	    }
	}
}
void HideCursor()
{
 CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
 SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void welcome(void)
{
	system("mode con cols=40 lines=28");
	HideCursor();
	srand(time(NULL));
	pos(7,18);
	printf("welcome");
	memset(map,0,sizeof(map));
	select(); 
}
