#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFile>
int tabla(int estado,int columna);
int columna(char c);
QString token(int e,QString caracteres);
QString* analiza(QString cadena);
QString error(int e,QString caracteres);
QString dirArchivo="";
bool e = false;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAnalizar_clicked()
{   
     QString* cadena_final=analiza(ui->textEditor->toPlainText());
         ui->textErrores->setText(cadena_final[1]);
       ui->textConsola->setText(cadena_final[0]);

}
void MainWindow::on_btnAbrir_clicked()
{
    dirArchivo=QFileDialog::getOpenFileName(
    this,
    tr("Abrir el archivo"),// file Title
    "C://",// predetermined opening route
    "Archivo COVID (*.cvd);; All files (*.*)" //shows COVID File types (filtered)
  );
     if(dirArchivo==""){
           QMessageBox::information(this,tr("Alerta"),"Ningun archivo se selecciono");
       }else{
          QFile file (dirArchivo);
          if(!file.open(QIODevice::ReadOnly)){
              QMessageBox::information(0,"info",file.errorString()); //if for some reason the file doesnt open
               }else{
              QTextStream in(&file);//everything thats in the file get copied
              ui->textEditor->setText(in.readAll());//than gets shown on the text editor
          }
       }
}

void MainWindow::on_btn_guardar_clicked()
{
    QString archivo = QFileDialog::getSaveFileName(this,
                                                 tr("Guardar archivo"),"",
                                                 tr("Archivo COVID (*.cvd);; Todos los archivos(*.*)"));
     QFile file(archivo);
     if(file.open(QFile::WriteOnly)){

         file.write(ui->textEditor->toPlainText().toUtf8());
         QMessageBox::information(this,tr("Guardado"),"Archivo guardado");
     }else{
         QMessageBox::warning(
                     this,
                     "Text editor",
                     tr("No se puede escribir en el archivo %1.\n Error %2")
                     .arg(dirArchivo)
                     .arg(file.errorString()));

     }
}

QString* analiza(QString cadena){
    int estado=0,colum,i=0,tama=0;
    char caracter;
    QString caracteres,impresion;
    QString* prueba= new QString[2];
        cadena+="?";
        char* cadena_char = new char[cadena.length()];//a char* is created with the string length to save the string.
        tama=cadena.length();
        strcpy(cadena_char, cadena.toStdString().c_str()); //copies the content of the string to the string_char to have separate characters.
    do {
    while(estado<=17){
       if (!(i== tama-1 && cadena_char[tama-1] == '?')){ // (1) if its not the last character than do this.
           caracter=cadena_char[i];        
                     // (1) if it is the last character than do this.
       }else if(estado==0){//si es estado 0 entonces termina
          i++; break;
       }else{  //if it is the character? than (this is so it can accept it as different and proceed to final state)
           caracter=' ';
       }
        i++;
        colum= columna(caracter);
        estado=tabla(estado,colum);
       if (estado == 118 || estado==116 || estado ==114 ||estado == 102 || estado ==101 || estado== 104|| estado ==100 || estado ==113){ // if it is one of this states than the cicle is broken and it decreases by one i (esto con al finalidad de
           i--;break;                                                                                                                    // if "<<" is entered do not accept or save the second "<" otherwise it will form part of the final state.
        }                                                                                                                                // this will print that "<<" is a rational operator when in reality "<" is the rational operator, in this case if we take the second "<"
                                                                                                                                         // it will think of it as a different variable and adds it to the character variables and it also assumes that it has been analized when in fact it is another state that should be analyzed.
       if (estado==9){ //if it is state 9 it's beacuase it is a string, therefore all blank spaces will be considered part of the string.
          caracteres +=caracter;
       } else if(!(caracter==' '  || caracter==9 || caracter==10 )){   //if the character is a blank space, tab or line skip than it wont accept it.
                     caracteres +=caracter;
                }
    }
  if (estado>=100 && estado <=131){
          impresion +=token(estado,caracteres);
           caracteres="";
           estado=0;
       e=false;
  }else{
       if (estado>=500){

           e=true;
           prueba[0]=impresion;
           prueba[1]=error(estado,caracteres);
           return prueba;break;
       // return error(estado,caracteres);break;
       }
  }
  }while(i<tama );
        prueba[0]=impresion;
        return prueba;
}
int tabla(int estado,int columna){
int tabla[18][33]={{2,1,3,2,1,107,108,109,110,11,11,12,10,13,14,15,123,122,126,127,124,125,16,9,128,129,130,131,500,0,0,0,500},
               {2,1,2,2,1,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,2,100,100,100,100},
               {2,2,2,2,2,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,2,101,101,101,101},
               {102,102,3,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,4,102,102,102,102,102,102,102},
               {501,501,5,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501},
               {103,103,5,6,6,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
               {502,502,8,502,502,7,7,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502},
               {503,503,8,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503},
               {104,104,8,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104},
               {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,106,9,9,9,9,9,9,9,9,9},
               {113,113,113,113,113,113,113,113,113,113,113,113,112,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113},
               {114,114,114,114,114,114,114,114,114,114,114,114,115,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114},
               {116,116,116,116,116,116,116,116,116,116,116,116,117,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116},
               {118,118,118,118,118,118,118,118,118,118,118,118,119,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118},
               {505,505,505,505,505,505,505,505,505,505,505,505,505,505,120,505,505,505,505,505,505,505,505,505,505,505,505,505,505,505,505,505,505},
               {506,506,506,506,506,506,506,506,506,506,506,506,506,506,506,121,506,506,506,506,506,506,506,506,506,506,506,506,506,506,506,506,506},
               {17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,504,17,17,17,17,17,17,17,17,17,17},
               {507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,105,507,507,507,507,507,507,507,507,507,507}};

           return tabla[estado][columna];
 }//Fin de tabla
int columna(char c){
if (c>=65 && c<=90 && c!= 69){
    return 0;
}else if(c>=97 && c<=122 && c!=101){
    return 1;
}else if(c>=48 && c<=57){
    return 2;
}

switch(c){
    case 'E':return 3;break;
    case 'e':return 4;break;
    case '+':return 5;break;
    case '-':return 6;break;
    case '*':return 7;break;
    case '/':return 8;break;
    case '%':return 9;break;
    case '>':return 10;break;
    case '<':return 11;break;
    case '=':return 12;break;
    case '!':return 13;break;
    case '&':return 14;break;
    case '|':return 15;break;
    case '(':return 16;break;
    case ')':return 17;break;
    case '[':return 18;break;
    case ']':return 19;break;
    case '{':return 20;break;
    case '}':return 21;break;
    case 39 :return 22;break;
    case '"':return 23;break;
    case ';':return 24;break;
    case '.':return 25;break;
    case ':':return 26;break;
    case ',':return 27;break;
    case '_':return 28;break;
    case 32 :return 29;break;
    case 9  :return 30;break;
    case 10 :return 31;break;
    default: return 32;
}
} //Fin de columna

QString token(int e,QString caracteres){
    switch (e)
    {

    case 100:
        if((caracteres=="class" || caracteres=="for" ||caracteres=="while" ||caracteres=="do" ||caracteres=="if" ||caracteres=="then" ||
           caracteres=="else" || caracteres=="elseif" || caracteres=="switch" || caracteres=="case" || caracteres=="def" || caracteres=="import" ||
           caracteres=="int" || caracteres=="float" || caracteres=="char" || caracteres=="string" || caracteres=="input" || caracteres=="print")){
     return "Estado 100: " + caracteres +" Palabra Reservada" +"\r\n" ; }
    case 101:return "Estado 101: " + caracteres +" Identificador" +"\r\n" ;   //cout<<caracteres <<" Identificador"<<endl;break;
    case 102:return "Estado 102: " + caracteres +" Constante entera" +"\r\n" ;//cout<<caracteres <<" Constante entera"<<endl;break;
    case 103:return "Estado 103: " + caracteres +" Constante flotante" +"\r\n" ;//cout<<caracteres <<" Constante flotante"<<endl;break;
    case 104:return "Estado 104: " + caracteres +" Constante notacion cientifica" +"\r\n" ; //cout<<caracteres <<" Constante notacion cientifica"<<endl;break;
    case 105:return "Estado 105: " + caracteres +" Caracter" +"\r\n" ;//cout<<caracteres <<" Caracter"<<endl;break;
    case 106:return "Estado 106: " + caracteres +" Cadena " +"\r\n" ;//cout<<caracteres <<" Cadena"<<endl;break;
    case 107:return "Estado 107: " + caracteres +" Operador " +"\r\n" ;
    case 108:return "Estado 108: " + caracteres +" Operador " +"\r\n" ;
    case 109:return "Estado 109: " + caracteres +" Operador " +"\r\n" ;
    case 110:return "Estado 110: " + caracteres +" Operador " +"\r\n" ;
    case 111:return "Estado 111: " + caracteres +" Operador " +"\r\n" ; //cout<<caracteres <<" Operador"<<endl;break;
    case 112:return "Estado 112: " + caracteres +" Operador relacional"+"\r\n";
    case 113:return "Estado 113: " + caracteres +" Operador de asignacion "+"\r\n";
    case 114:return "Estado 114: " + caracteres +" Operador relacional " +"\r\n";
    case 115:return "Estado 115: " + caracteres +" Operador relacional " +"\r\n";
    case 116:return "Estado 116: " + caracteres +" Operador relacional " +"\r\n";
    case 117:return "Estado 117: " + caracteres +" Operador relacional " +"\r\n";
    case 118:return "Estado 118: " + caracteres +" Operador logico " +"\r\n";
    case 119:return "Estado 119: " + caracteres +" Operador relacional " +"\r\n";//cout<<caracteres <<" Operador relacional"<<endl;break;
    case 120:return "Estado 120: " + caracteres +" Operador logico " +"\r\n";
    case 121:return "Estado 121: " + caracteres +" Operador logico " +"\r\n";//cout<<caracteres <<" Operador Logico"<<endl;break;
    case 122:return "Estado 122: " + caracteres +" Agrupador " +"\r\n";
    case 123:return "Estado 123: " + caracteres +" Agrupador " +"\r\n";
    case 124:return "Estado 124: " + caracteres +" Agrupador " +"\r\n";
    case 125:return "Estado 125: " + caracteres +" Agrupador " +"\r\n";
    case 126:return "Estado 126: " + caracteres +" Agrupador " +"\r\n";
    case 127:return "Estado 127: " + caracteres +" Agrupador " +"\r\n";//cout<<caracteres <<" Agrupador"<<endl;break;
    case 128:return "Estado 128: " + caracteres +" Signo de puntuacion " +"\r\n";
    case 129:return "Estado 129: " + caracteres +" Signo de puntuacion " +"\r\n";
    case 130:return "Estado 130: " + caracteres +" Signo de puntuacion " +"\r\n";
    case 131:return "Estado 131: " + caracteres +" Signo de puntuacion " +"\r\n"; //cout<<caracteres <<" Signo de puntuacion"<<endl;break;
    default: return caracteres + " Caracter no reconocido " +"\r\n" ;//cout<<caracteres <<" Caracter no reconocido"<<endl;break;
    }

}//Fin de token

QString error(int e,QString caracteres){
       switch (e){
       case 500:return "Error 500:Ese elemento no es reconocido por el lenguaje : "+ caracteres; //cout<<"Error 500:Ese elemento no es reconocido por el lenguaje :" <<caracteres;break; //funciona
       case 501:return "Error 501:Valor flotante espera digitos :" + caracteres; //cout<<"Error 501:Valor flotante espera digitos :"<<caracteres;break; //funciona
       case 502:return "Error 502:Para la notacion cientifica, se esperaba dijito, + o - : " + caracteres;//cout<<"Error 502:Para la notacion cientifica, se esperaba dijito, + o - :"<<caracteres;break;//funciona
       case 503:return "Error 503:Para la notacion cientifica se esperaba dijito : " + caracteres;//cout<<"Error 503:Para la notacion cientifica se esperaba dijito :"<<caracteres;break;   //funciona
       case 504:return "Error 504:Se esperaba un caracter diferente a comilla simple  : " + caracteres;//cout<<"Error 504:Se esperaba un caracter diferente a comilla simple  :"<<caracteres;break; // funciona
       case 505:return "Error 505:Se esperaba un &  : " + caracteres;//cout<<"Error 505:Se esperaba un &  :"<<caracteres;break; //funciona
       case 506:return "Error 506:Se esperaba un |  : " + caracteres;//cout<<"Error 506:Se esperaba un |  :"<<caracteres;break; //funciona
       case 507:return "Error 507:Se esperaba comilla simple :  " + caracteres;//cout<<"Error 507:Se esperaba comilla simple : "<<caracteres;break;
       }
}// Fin de error



