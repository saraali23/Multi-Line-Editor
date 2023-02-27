#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <windows.h>

int main()
{
    int size;
    printf(" Enter Number of lines :");
    scanf("%d",&size);
    char * lines[size];
    editor(lines,size,11);

    free(lines);
    return 0;
}
//helper functions
void textattr(int i){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);

}
char check_norm_extended(char k,int *ex){//check if pressed key is normal or extended


    if(k==-32){
        k=getch();
        *ex=0;
    }
    else{
        *ex=1;
        }

    return k;

}
void gotoxy(short x, short y){ //col,row
    COORD pos = {x,y};//sets co-ordinates in(x,y)
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
void print_line(int size,char *sts[],char *ends[],int row, short  col,int lines_no){ //0 left 1 right


    char *temp;
    for(int j=0;j<lines_no;j++){
            gotoxy(20,(short)(10+(j*2)));
            temp=sts[j];
            for(int i=0;i<size;i++){
                     textattr(0x64);
                     if(temp<=ends[j]){
                        _cprintf("%c",*temp);
                        temp++;
                    }
                   else
                        _cprintf(" ");

            }

    }

    textattr(0x07);
    gotoxy(col,(short)(10+(row*2)));

}
void shift(char *start,char *end,int left){
    char *temp;
    char t;
    if(left=1){ //SHIFT LEFT
        temp=start;
        temp++;
        while(temp!=end){

            *start=*temp;
            temp++;
            start++;
        }
        *start=*temp;
    }
    else{  //SHIFT RIGHT
        temp=end;
        temp++;
        while(end!=start){
            *temp=*end;
            temp--;
            end--;
        }
        *temp=*end;

    }


}
void editor(char * lines[],int lines_no,int size){

  char * sts[lines_no];
  char * ends[lines_no];
  short  cols[lines_no];
  short  endcols[lines_no]; //indicator to end of word
  int index=0;//current line index
  int write=0;

  for(int i=0;i<lines_no;i++){
        lines[i]=(char *)malloc((size+1)*sizeof(char));
        if(!lines[i])exit(1);
        sts[i]=(char *)malloc(sizeof(char));
        if(!sts[i] )exit(1);
        sts[i]=lines[i];
        ends[i]=(char *)malloc(sizeof(char));
        if(!ends[i])exit(1);
         ends[i]=lines[i]+(1*sizeof(char));
         cols[i]=20;
        endcols[i]=21;



  }

  char *line=lines[index];
  char *curr=line;
   short static stcol=20;  //last row in current word
  //short static endcol=21; //last col in current word
  short static row=10;   //current row
  //short static col=20; //current col
  //char *end=line;
  //char *st = line;

  char key;
  int ex=0;//0 ex ,1 norm

  while(key!=27 && (key!=13 || ex!=1)){ //ESC OR ENTER
        print_line(size,sts,ends,index,cols[index],lines_no);
        key=getch();
        key=check_norm_extended(key,&ex);

        if(ex==0){//extended
                switch(key)
                {
                    case 77:
                    {//right arrow
                            if(cols[index]-20>=size-1)continue;

                            else if(curr<ends[index] ){ //in middle of line
                                curr++;
                                cols[index]++;
                            }
                            else if(curr==ends[index] & cols[index]<endcols[index]){//at end of current word
                                curr++;
                                cols[index]++;
                                ends[index]++;


                                continue;


                            }

                            else if(curr<sts[index]){
                                curr++;
                                cols[index]++;
                                ends[index]++;
                            }
                             if(cols[index]==endcols[index]) endcols[index]++;
                            /* else if(write==1){
                                curr++;
                                cols[index]++;
                                endcols[index]++;
                                write=0;


                             }
                             */

                            break;

                    }
                    case 75:
                    {//left arrow
                            if(curr>sts[index]){
                                curr--;
                                cols[index]--;
                            }

                    }        break;
                    case 73:
                    {//page up/home
                            curr=sts[index];
                            cols[index]=stcol;
                             break;

                    }
                    case 81:
                    {//page down/end
                            curr=ends[index];
                            //char_ind=size-1;
                            cols[index]=endcols[index];
                             break;
                    }
                    case 83: //delete

                    {       if(curr<sts[index]) {curr=sts[index];cols[index]=20; continue;}
                            if(curr==ends[index] & endcols[index]>cols[index] & curr!=sts[index] ){//at end of world not start
                                *curr=' ';
                                curr--;
                                cols[index]--;
                                ends[index]--;
                                endcols[index]--;
                            }

                            else if(curr>sts[index] & endcols[index]>cols[index]){//(middle of word)not space at end of word
                                *curr=' ';
                                char *temp1=curr;
                                char *temp2=ends[index];
                                shift(temp1,temp2,1);
                                curr--;
                                cols[index]--;
                                ends[index]--;
                                endcols[index]--;

                            }
                            else if(curr==sts[index] && curr< ends[index]){//at start of word
                                *curr=' ';
                                char *temp1=curr;
                                char *temp2=ends[index];
                                shift(temp1,temp2,1);
                                ends[index]--;
                                endcols[index]--;
                            }
                            else{

                                *curr=' ';
                            }


                            break;
                    }
                    case 72:{ //up
                        if(index<=0)continue;//first line
                        else{
                            index--;
                             curr=sts[index]+(sizeof(char)*(cols[index+1]-20));

                            if(curr>ends[index]){
                                curr=ends[index];
                                cols[index]=endcols[index]-1;
                                continue;

                            }
                            cols[index]=cols[index+1];

                        }
                        break;

                    }
                    case 80:{ //down
                        if(index==lines_no-1) continue;//last line
                        else{
                            index++;
                            curr=sts[index]+(sizeof(char)*(cols[index-1]-20));

                            if(curr>ends[index]){
                                curr=ends[index];
                                cols[index]=endcols[index]-1;
                                continue;

                            }
                             cols[index]=cols[index-1];

                        }
                        break;

                    }
                    default:
                    {
                        system("cls");
                        textattr(0x04);
                        printf("\n $ Enter valid key $  \n");
                        textattr(0x07);
                    }
            }

        }
        else{//normal
                switch(key){
                case 13:
                    {//enter
                            system("cls");
                            textattr(0x04);
                            line[size-1]='\0';
                            for(int i=0;i<lines_no;i++){
                                 printf("\n %s \n",lines[i]);
                            }

                            printf("\n $ ByeBye $  \n");
                            textattr(0x07);
                             break;

                    }
                    case 27:
                    {//escape
                            system("cls");
                            textattr(0x04);
                            line[size-1]='\0';
                            line[size-1]='\0';
                            for(int i=0;i<lines_no;i++){
                                 printf("\n %s \n",lines[i]);
                            }
                            printf("\n $ Salam $  \n");
                            textattr(0x07);
                             break;
                    }
                    case 8: //backspace
                    {
                           curr--;
                           if(curr<sts[index])
                            {   curr=sts[index];cols[index]=20; continue;
                                }
                           if(cols[index]>(endcols[index]-1)& curr!=sts[index] ){//at end of world not start
                                *curr=' ';
                                cols[index]--;
                                ends[index]--;
                                endcols[index]--;
                            }

                            else if(curr>sts[index] & endcols[index]>cols[index]){//(middle of word)not space at end of word
                                *curr=' ';
                                char *temp1=curr;
                                char *temp2=ends[index];
                                shift(temp1,temp2,1);
                                cols[index]--;
                                ends[index]--;
                                endcols[index]--;

                            }
                            else if(curr==sts[index] && curr< ends[index]){//at start of word
                                *curr=' ';
                                char *temp1=curr;
                                char *temp2=ends[index];
                                shift(temp1,temp2,1);
                                ends[index]--;
                                endcols[index]--;
                            }



                            break;

                    }
                    default:
                    {
                         //normal char
                         // if(curr==sts[index]&&curr==ends[index]) {ends[index]++;}
                           *curr=key;
                            write=1;
                            if(ends[index]>line[size-1] && curr>=ends[index])//end of word
                                continue;
                           if(ends[index]==curr && cols[index]==endcols[index])
                                {endcols[index]++;}//insert at end of line



                    }
              }
            }




        }


  }



