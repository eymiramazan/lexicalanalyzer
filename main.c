#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isKeyword(char words[]) //keyword kontrol?
{
    char keywords[18][10] = {"break", "case", "char", "const", "continue", "do", "else", "enum", "float", "for", "goto", "if", "int",
                            "long", "record", "return","static","while"};

    int i, flag = 0;

    for (i = 0; i < 18; i++)
    {
        if (strcmp(keywords[i], words) == 0)  //if its keyword set flag = 1
        {
            flag = 1;
            break;
        }
    }
    return flag;
}

int main()
{

    FILE *psi = fopen("code.psi", "r");
    FILE *lex = fopen("code.lex", "w");

    char word[20];

    if (!psi)  //read file
    {
        printf("File Not Found!\n");
        return 0;
    }

    printf("Reading File Successfully Completed!\n");

    char temp = fgetc(psi);  //reading file 

    while (!feof(psi))
    {
        if (temp == 32 || temp == '\n')  //skip whitespace
        {
            temp = fgetc(psi);
        }
        char number[10];
        if (47 < temp && temp < 58)    //integer control
        {
            int flag=5;
            int i = 0;
            number[i] = temp;
            i=1;
            temp = fgetc(psi);
            while ((47 < temp && temp < 58) && temp != '\n' && temp != '\0')  //if read char is int continues to read the rest  
            {
                if(i == 10){
                    fprintf(lex,"Integer size cannot be more than 10 digit!\n");
                    flag = 1;                                           //If its not IntConst 
                }

                number[i++] = temp;
                temp = fgetc(psi);

            }

            if ((64 < temp && temp < 91) || (96 < temp && temp < 123))   //the next character after reading the number becomes a letter
            {
                fprintf(lex,"Identifiers start with an alphabetic character!\n");
                flag = 1;                                                //If its not IntConst
                while ((64 < temp && temp < 91) || (96 < temp && temp < 123) || temp == 95 || 47 < temp && temp < 58)
                {
                    temp = fgetc(psi);
                }

            }
            number[i] = '\0';

            if (flag != 1)          //If its IntConst, generates a Token
            {
                fprintf(lex,"IntConst (%s)\n",number);

            }
        }

        int commentControl = 1;
        if (temp == '(')   //Comment and leftpar control
        {
            temp = fgetc(psi);
            if (temp == '*')
            {
                while (!feof(psi))
                {
                    temp = fgetc(psi);
                    if (temp == '*')
                    {
                        temp = fgetc(psi);
                        if (temp == ')')
                        {
                            temp = '0';        //avoid rightpar control
                            commentControl = 0;
                            break;

                        }
                    }

                    if (temp == EOF)  //comment dont end until end of file
                    {
                        fprintf(lex,"Comments Must Terminated Before The File End!\n");
                    }
                }
            }
            if (commentControl)  //if its not comment its leftpar and it generates Token
            {
                fprintf(lex,"LeftPar\n");
            }
        }
        if ((96 < temp && temp < 123) || (64 < temp && temp < 91))  //Identifier and keyword control
        {
            if (64 < temp && temp < 91 )  //capitalize lowercase
            {
                    temp += 32 ;
            }
            int flag = 0;
            int i = 0;
            int j = 0;
            word[i] = temp;
            i = 1;
            temp = fgetc(psi);

            while ((47 < temp && temp < 58) || temp == 95 || (64 < temp && temp < 91) || (96 < temp && temp < 123)) //letter int underscore
            {
                if (i == 20){
                    fprintf(lex,"Identifier size cannot be more than 20 character!\n");
                }
                if ((47 < temp && temp < 58) || temp == 95){ //if it contains underscore and int it cannot be keyword
                    flag = 1;
                }

                if (64 < temp && temp < 91 ) //capitalize lowercase
                {
                    temp += 32 ;
                }

                word[i++] = temp;
                temp = fgetc(psi);

            }
            if (temp == 33 || (34 <temp && temp < 42) || temp == 44 || temp == 46 || temp == 60 || (61 < temp && temp < 65) || (90 < temp && temp < 95) || temp == 96 || (122 < temp && temp < 127))
            {
                fprintf(lex,"Identifiers can only contain letters, numbers and underscore(_)!\n");
                flag = 2;
                while ((47 < temp && temp < 58) ||temp == 33 || (34 <temp && temp < 42) || temp == 44 || temp == 46 || temp == 60 || (61 < temp && temp < 65) || (90 < temp && temp < 95) || temp == 96 || (122 < temp && temp < 127) || (64 < temp && temp < 91) || (96 < temp && temp < 123))
                {
                    temp = fgetc(psi);
                }

            }

            word[i] = '\0';

            if (flag == 0 && flag != 2)  //if it is not contain unacceptable characters check is it keyword
            {
                if (isKeyword(word))
                    fprintf(lex,"Keyword (%s)\n", word);
                else
                    flag = 1;
            }

            if (flag == 1 && i < 21 && flag !=2){ //Identifier Token
                for (int q = 0; q < sizeof(word); q++)
                {
                    word[q] = toupper(word[q]);
                }

                fprintf(lex,"Identifier (%s)\n", word);
            }
        }

        //Operators
        if (temp == '+')
        {
            temp = fgetc(psi);
            if (temp == '+')
            {
                fprintf(lex,"Operator (++)\n");
            }
            else
            {
                fprintf(lex,"Operator (+)\n");
                continue;
            }
        }

        if (temp == '-')
        {
            temp = fgetc(psi);
            if (temp == '-')
            {
                fprintf(lex,"Operator (--)\n");
            }
            else
            {
                fprintf(lex,"Operator (-)\n");
                continue;
            }
        }

        if (temp == '*')
        {
            fprintf(lex,"Operator (*)\n");
        }

        if (temp == '/')
        {
            fprintf(lex,"Operator (/)\n");
        }

        if (temp == ':')
        {
            temp = fgetc(psi);
            if (temp == '=')
            {
                fprintf(lex,"Operator (:=)\n");
            }
        }

        if (temp == ';')
        {
            fprintf(lex,"EndOfLine\n");
        }

        if (temp == ')')
        {
            fprintf(lex,"RightPar\n");
        }

        if (temp == '[')
        {
            fprintf(lex,"LeftSquareBracket\n");
        }

        if (temp == ']')
        {
            fprintf(lex,"RightSquareBracket\n");
        }

        if (temp == '{')
        {
            fprintf(lex,"LeftCurlyBracket\n");
        }

        if (temp == '}')
        {
            fprintf(lex,"RightCurlyBracket\n");
        }


        //String
        if (temp == '"')
        {
            while (!feof(psi))
            {
                temp = fgetc(psi);
                if (temp == '"')
                {
                    break;
                }

                if (temp == EOF)  //give an error if the string is not completed until the end of the file
                {
                    fprintf(lex,"Strings Must Terminated Before The File End!");
                    break;
                }
            }
        }

        temp = fgetc(psi);
    }
    fclose(psi);
    fclose(lex);
    return 0;
}
