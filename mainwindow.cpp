#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFile>
int tabla(int estado,int columna);
int columna(char c);

template<class T, size_t N>
constexpr size_t size(T (&)[N]) { return N; }
QString token(int e,QString caracteres);
QString* analiza(QString cadena,int j);
QString Sintactic_Analizer(QString file);
QString error(int e,QString caracteres);
int colum(QString token,int estado);
int Matriz_colum(QString token,int estado);
int PredictiveMatriz(int file,int colum);
int* Productions(int X);
QString dirArchivo="";
#define GetSize(array_enteros) (sizeof(array_enteros)/sizeof(*(array_enteros)))
using namespace std;
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
    //QString* cadena_final=analiza(ui->textEditor->toPlainText());
      //   ui->textErrores->setText(cadena_final[1]);
      // ui->textConsola->setText(cadena_final[0]);
    ui->textConsola->setText(Sintactic_Analizer(ui->textEditor->toPlainText()));

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

QString* analiza(QString cadena,int j){
    int estado=0,colum,i=j,tama=0;
    char caracter;
    QString caracteres,impresion,caracter2="";
    QString* prueba= new QString[6];
        cadena+="$";
        char* cadena_char = new char[cadena.length()];//a char* is created with the string length to save the string.
        tama=cadena.length();
        if (i==tama-1){
            prueba[3]="$";
            return prueba;
        }
        strcpy(cadena_char, cadena.toStdString().c_str()); //copies the content of the string to the string_char to have separate characters.
  //  do {
    while(estado<=17){
       if (!(i== tama-1 && cadena_char[tama-1] == '$')){ // (1) if ($) it's not the last character than do this.
           caracter=cadena_char[i];        
                     // (1) if it is the last character than do this.
       }else if(estado==0){//si es estado 0 entonces termina
          i++;
          if (i-1==tama-1){
              prueba[3]="$";
              return prueba;
          }break;

       }else{  //if it is the character $ than (this is so it can accept it as different and proceed to final state)
           caracter=' ';

       }
        i++;
        colum= columna(caracter);
        estado=tabla(estado,colum);
       if (estado == 118 || estado==116 || estado ==114 || (estado >= 100 && estado <=104) || estado ==113){ // if it is one of this states than the cicle is broken and it decreases by one i (esto con al finalidad de
           i--;
          break;                                                                                                                    // if "<<" is entered do not accept or save the second "<" otherwise it will form part of the final state.
        }                                                                                                                                // this will print that "<<" is a rational operator when in reality "<" is the rational operator, in this case if we take the second "<"
                                                                                                                                         // it will think of it as a different variable and adds it to the character variables and it also assumes that it has been analized when in fact it is another state that should be analyzed.
       if (estado==9){ //if it is state 9 it's becuase it is a string, therefore all blank spaces will be considered part of the string.
          caracteres +=caracter;
       } else if(!(caracter==' '  || caracter==9 || caracter==10 )){   //if the character is a blank space, tab or line skip than it wont accept it.
                     caracteres +=caracter;
                }
    }//end while
  if (estado>=100 && estado <=131){
          impresion +=token(estado,caracteres);
           prueba[0]=impresion;
           prueba[1]="";
           prueba[2]="true";
           prueba[3]=caracteres;       
           prueba[4]=QString::number(i);//para comnvertir int a string
           prueba[5]=QString::number(estado);
           caracteres="";
           estado=0;
           return prueba;

  }else{
       if (estado>=500){          
           prueba[0]=impresion;
           prueba[1]=error(estado,caracteres);
           prueba[2]="error";

           //->textErrores->setText(cadena_final[1]);
           return prueba;//break;
       // return error(estado,caracteres);break;
       }
  }
 // }while(i<tama );//end do while
        prueba[0]=impresion+"--------";
        prueba[1]="";
        prueba[2]="true";
        prueba[3]=caracteres;
        prueba[4]=QString::number(i);//para convertir int a string
        prueba[5]=QString::number(estado);

        return prueba;
}
QString Sintactic_Analizer(QString file){
   static int* pilaAux = new int[11];
  int* pila = new int[1000];
   pila[0]=1057;// in has $
   pila[1]=1; // in has Program
     int i=1; //puntero donde dice en que posicion esta el ultimo elemento de la pila
     int j=0;//indicador que ayuda a saber al lexico donde quedo
     int l=i+1; //puntero que indica a que posicion debe irse el siguiente elemento de la pila
     int X=pila[1]; // X= Program
     int estado=0;
  QString nexttoken="";
  QString* tokens= new QString[6]; // aqui guardo los elementos enviados por el lexico

 QString correct_msg;// aqui se guarda la impresion correcta del lexico
  QString error_msg;//aqui se guardan los errores del lexico
  int correct;
 do{
      if(nexttoken == ""){
          tokens=analiza(file,j); //se le manda el archivo a analiza
                  correct_msg+=tokens[0];
                 error_msg=tokens[2];
                  nexttoken=tokens[3];
                  estado=tokens[5].toInt();
                  j=tokens[4].toInt();
          correct = QString::compare(error_msg,"error", Qt::CaseInsensitive);// si las cadenas son iguales entonces devuelve 0
          if (correct == 0){ // si son iguales, quiere decir que hay un error en el lexico
              error_msg= tokens[1];
              return error_msg;

          }

      }
      if(pila[i]>=1 && pila[i]<= 61){//es terminal
          X= pila[i]-1;
      }else{                            // es no terminal
          X= pila[i];
      }
      if(pila[i]==601){
          i--; //quito el top de la pila
          l=i+1; //indico la siguiente posicion de la pila
          X= pila[i];

      }
  if(X>=1000 && X<= 1061){ // it´s terminal

      if(X==colum(nexttoken,estado) && X== 1057){
          correct_msg +="------------- Archivo correcto sintacticamente -------------";
          pila[0]=0;
          return correct_msg;
      }else if(X==colum(nexttoken,estado) && X != 1057){
          i--; //quito el top de la pila
          l=i+1; //indico la siguiente posicion de la pila
          nexttoken=""; //  quito nexttoken de la entrada
      }else {
          error_msg+= "Error, se esperaba un estado terminal \r\n" + nexttoken;

          return error_msg;
      }

  }else{ // it´s No-terminal
    //  int subdev=pila[i]-1;
    //  int subdev2=Matriz_colum(nexttoken,estado);

      int derivada=PredictiveMatriz(pila[i]-1,Matriz_colum(nexttoken,estado));

     if( derivada>= 1 && derivada <= 103 ){ // if it`s correct, means that we have a derivate
           i--; //quito el top de la pila
           l=i+1;//indico la siguiente posicion de la pila
           pilaAux=Productions(derivada); // aqui guardo las derivadas

           for(int k=pilaAux[10]-1;k>=0;k--){
                pila[l]=pilaAux[k];    
                i++;// aumento el puntero del top
                l=i+1; //indico siguiente posicion hacia top
              }
     }else{
          error_msg+= "Error, se esperaba alguno de los first en "+ nexttoken+ "\r\n";
         return error_msg;
     }

}
  }while (!(nexttoken =="$" && pila[0]==0));
}

//{0,1000,1002,1003,1004,1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,1022,1023,1024,1025,1026,1027,1028,1029,1030,1031,1032,1033,1034,1035,1036,1037,1038,1039,1040,1041,1042,1043,1044,1045,1046,1047,1048,1049,1050,1051,1052,1053,1054,1055,1056,1057,1058,1059,1060,1061},
int PredictiveMatriz(int file,int colum){
    int table[46][62]={
                       {2,1,600,1,600,1,600,1,600,1,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,3,600,4,600,4,600,4,600,4,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,4,600,600,600,600},
                       {600,600,6,5,600,6,600,6,600,6,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,6,600,600,600,600},
                       {600,600,600,600,8,600,600,600,600,600,600,600,7,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,8,600,600,600,600},
                       {600,600,10,600,600,9,600,10,600,10,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,10,600,600,600,600},
                       {600,600,600,600,600,600,12,600,600,600,600,600,11,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,12,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,13,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,14,600,14,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,15,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,17,600,600,600,600,16,600,17,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,17,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,18,19,20,21,22,23,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,26,27,28,24,25,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,29,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,30,600,600,30,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,31,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,32,600,32,600,600,600,600},
                       {600,600,600,600,600,600,600,600,34,600,600,600,34,600,600,600,600,600,600,600,600,600,600,600,600,33,600,34,600,600,600,34,600,34,600,34,34,34,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,34,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,35,600,600,600,600,600,600,600,600,600,600,600,600,600,36,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,36,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,37,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,38,600,38,600,600,600,600},
                       {600,600,600,600,600,600,600,600,40,600,600,40,39,600,600,600,600,600,600,600,600,600,600,600,600,600,600,39,40,40,40,39,40,39,40,39,39,39,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,40,600,600,600,600},
                       {600,600,600,600,600,600,600,600,48,600,600,48,41,600,600,600,600,600,600,600,600,600,600,600,600,600,600,42,600,48,48,43,48,44,48,45,46,47,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,48,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,49,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,50,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,51,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,52,600,600,52,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,53,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,54,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,55,600,600,55,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,57,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,57,57,600,600,57,57,57,57,57,57,57,57,57,57,57,56,600,57,57,57,600,600,600,57},
                       {600,600,600,600,600,600,600,600,600,600,600,600,58,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,59,600,600,59,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,60,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,61,600,600,61,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,62,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,64,63,64,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,64,600,600,600,600},
       //              {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,64,63,64,64,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,64,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,66,600,65,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,66,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,67,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,68,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,69,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,70,600,600,600,600,600,600,600,70,70,70,70,70,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,70,600,600,600,600,600,600,600,600,600,600,600,600,70,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,72,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,71,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,72,72,72,72,600,600,600,72},
                       {600,600,600,600,600,600,600,600,600,600,600,600,73,600,600,600,600,600,600,600,73,73,73,73,73,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,73,600,600,600,600,600,600,600,600,600,600,600,600,73,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,75,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,75,74,600,600,600,600,600,600,600,600,600,600,600,600,600,600,75,75,75,600,600,600,600,75},
                       {600,600,600,600,600,600,600,600,600,600,600,600,76,600,600,600,600,600,600,600,76,76,76,76,76,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,76,600,600,600,600,600,600,600,600,600,600,600,600,76,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,78,600,600,600,600,600,600,600,78,78,78,78,78,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,77,600,600,600,600,600,600,600,600,600,600,600,600,78,600,600,600,78,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,79,600,600,600,600,600,600,600,79,79,79,79,79,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,79,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,81,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,81,81,600,600,80,80,80,80,80,80,600,600,600,600,600,600,81,81,81,81,600,600,600,81},
                       {600,600,600,600,600,600,600,600,600,600,600,600,82,600,600,600,600,600,600,600,82,82,82,82,82,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,82,600,600,600,82,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,85,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,85,85,600,600,85,85,85,85,85,85,83,84,600,600,600,600,85,85,85,85,600,600,600,85},
                       {600,600,600,600,600,600,600,600,600,600,600,600,86,600,600,600,600,600,600,600,86,86,86,86,86,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,86,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,90,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,90,90,600,600,90,90,90,90,90,90,90,90,87,88,89,600,90,90,90,90,600,600,600,90},
                       {600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,91,92,93,94,95,96,600,600,600,600,600,600,600,600,600,600,600,600,600,600},
                       {600,600,600,600,600,600,600,600,600,600,600,600,97,600,600,600,600,600,600,600,100,101,102,98,99,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,600,103,600,600,600,600,600,600,600,600}};

    return table[file][colum];
}

int* Productions(int X){
    static int* pila = new int[11];
    switch (X) {
     case 1: pila[0]=2;pila[1]=3;pila[2]=5;pila[3]=9;pila[4]=8;pila[10]=5;break;
     case 2: pila[0]=1000;pila[1]=3;pila[2]=5;pila[3]=9;pila[4]=1002;pila[10]=5;break;
     case 3: pila[0]=1001;pila[1]=1012;pila[2]=1056;pila[3]=2;pila[10]=4;break;
     case 4: pila[0]=601;pila[10]=1;break;
     case 5: pila[0]=1003;pila[1]=4;pila[2]=1004;pila[10]=3;break;
     case 6: pila[0]=601;pila[10]=1;break;
     case 7: pila[0]=1012;pila[1]=1041;pila[2]=11;pila[3]=1056;pila[4]=4;pila[10]=5;break;
     case 8: pila[0]=601;pila[10]=1;break;
     case 9: pila[0]=1005;pila[1]=6;pila[2]=1006;pila[10]=3;break;
     case 10: pila[0]=601;pila[10]=1;break;
     case 11: pila[0]=1012;pila[1]=7;pila[2]=1055;pila[3]=10;pila[4]=1056;pila[5]=6;pila[10]=6;break;
     case 12: pila[0]=601;pila[10]=1;break;
     case 13: pila[0]=1013;pila[1]=1012;pila[2]=7;pila[10]=3;break;
     case 14: pila[0]=601;pila[10]=1;break;
     case 15: pila[0]=1009;pila[1]=1010;pila[2]=17;pila[3]=1011;pila[10]=4;break;
     case 16: pila[0]=1007;pila[1]=1012;pila[2]=1055;pila[3]=10;pila[4]=1053;pila[5]=12;pila[6]=1054;pila[7]=14;pila[8]=17;pila[9]=1008;pila[10]=10;break;
     case 17: pila[0]=601;pila[10]=1;break;
     case 18: pila[0]=1014;pila[10]=1;break;
     case 19: pila[0]=1015;pila[10]=1;break;
     case 20: pila[0]=1016;pila[10]=1;break;
     case 21: pila[0]=1017;pila[10]=1;break;
     case 22: pila[0]=1018;pila[10]=1;break;
     case 23: pila[0]=1019;pila[10]=1;break;
     case 24: pila[0]=1023;pila[10]=1;break;
     case 25: pila[0]=1024;pila[10]=1;break;
     case 26: pila[0]=1020;pila[10]=1;break;
     case 27: pila[0]=1021;pila[10]=1;break;
     case 28: pila[0]=1022;pila[10]=1;break;
     case 29: pila[0]=1012;pila[1]=13;pila[2]=1055;pila[3]=10;pila[4]=1056;pila[5]=12;pila[10]=6;break;
     case 30: pila[0]=601;pila[10]=1;break;
     case 31: pila[0]=1013;pila[1]=1012;pila[2]=13;pila[10]=3;break;
     case 32: pila[0]=601;pila[10]=1;break;
     case 33: pila[0]=1025;pila[1]=15;pila[2]=1026;pila[10]=3;break;
     case 34: pila[0]=601;pila[10]=1;break;
     case 35: pila[0]=1012;pila[1]=16;pila[2]=1055;pila[3]=10;pila[4]=1056;pila[5]=15;pila[10]=6;break;
     case 36: pila[0]=601;pila[10]=1;break;
     case 37: pila[0]=1013;pila[1]=1012;pila[2]=16;pila[10]=3;break;
     case 38: pila[0]=601;pila[10]=1;break;
     case 39: pila[0]=18;pila[10]=1;break;
     case 40: pila[0]=601;pila[10]=1;break;
     case 41: pila[0]=19;pila[1]=1056;pila[2]=17;pila[10]=3;break;
     case 42: pila[0]=27;pila[1]=1056;pila[2]=17;pila[10]=3;break;
     case 43: pila[0]=31;pila[1]=1056;pila[2]=17;pila[10]=3;break;
     case 44: pila[0]=32;pila[1]=1056;pila[2]=17;pila[10]=3;break;
     case 45: pila[0]=30;pila[1]=1056;pila[2]=17;pila[10]=3;break;
     case 46: pila[0]=20;pila[1]=1056;pila[2]=17;pila[10]=3;break;
     case 47: pila[0]=22;pila[1]=1056;pila[2]=17;pila[10]=3;break;
     case 48: pila[0]=601;pila[10]=1;break;
     case 49: pila[0]=1012;pila[1]=1041;pila[2]=33;pila[10]=3;break;
     case 50: pila[0]=1036;pila[1]=1053;pila[2]=1012;pila[3]=21;pila[4]=1054;pila[10]=5;break;
     case 51: pila[0]=1013;pila[1]=1012;pila[2]=21;pila[10]=3;break;
     case 52: pila[0]=601;pila[10]=1;break;
     case 53: pila[0]=1037;pila[1]=1053;pila[2]=33;pila[3]=23;pila[4]=1054;pila[10]=5;break;
     case 54: pila[0]=1013;pila[1]=33;pila[2]=23;pila[10]=3;break;
     case 55: pila[0]=601;pila[10]=1;break;
     case 56: pila[0]=1053;pila[1]=25;pila[2]=1054;pila[10]=3;break;
     case 57: pila[0]=601;pila[10]=1;break;
     case 58: pila[0]=1012;pila[1]=26;pila[10]=2;break;
     case 59: pila[0]=601;pila[10]=1;break;
     case 60: pila[0]=1013;pila[1]=1012;pila[2]=26;pila[10]=3;break;
     case 61: pila[0]=601;pila[10]=1;break;
     case 62: pila[0]=1027;pila[1]=33;pila[2]=1055;pila[3]=17;pila[4]=28;pila[5]=29;pila[6]=1028;pila[10]=7;break;
     case 63: pila[0]=1029;pila[1]=33;pila[2]=1055;pila[3]=17;pila[4]=28;pila[10]=5;break;
     case 64: pila[0]=601;pila[10]=1;break;
     case 65: pila[0]=1030;pila[1]=1055;pila[2]=17;pila[10]=3;break;
     case 66: pila[0]=601;pila[10]=1;break;
     case 67: pila[0]=1035;pila[1]=33;pila[10]=2;break;
     case 68: pila[0]=1031;pila[1]=33;pila[2]=1055;pila[3]=17;pila[4]=1032;pila[10]=5;break;
     case 69: pila[0]=1033;pila[1]=1012;pila[2]=1041;pila[3]=33;pila[4]=1061;pila[5]=33;pila[6]=1055;pila[7]=17;pila[8]=1034;pila[10]=9;break;
     case 70: pila[0]=35;pila[1]=34;pila[10]=2;break;
     case 71: pila[0]=1038;pila[1]=33;pila[10]=2;break;
     case 72: pila[0]=601;pila[10]=1;break;
     case 73: pila[0]=37;pila[1]=36;pila[10]=2;break;
     case 74: pila[0]=1039;pila[1]=35;pila[10]=2;break;
     case 75: pila[0]=601;pila[10]=1;break;
     case 76: pila[0]=38;pila[1]=39;pila[10]=2;break;
     case 77: pila[0]=1040;pila[10]=1;break;
     case 78: pila[0]=601;pila[10]=1;break;
     case 79: pila[0]=41;pila[1]=40;pila[10]=2;break;
     case 80: pila[0]=45;pila[1]=41;pila[10]=2;break;
     case 81: pila[0]=601;pila[10]=1;break;
     case 82: pila[0]=43;pila[1]=42;pila[10]=2;break;
     case 83: pila[0]=1048;pila[1]=41;pila[10]=2;break;
     case 84: pila[0]=1049;pila[1]=41;pila[10]=2;break;
     case 85: pila[0]=601;pila[10]=1;break;
     case 86: pila[0]=46;pila[1]=44;pila[10]=2;break;
     case 87: pila[0]=1050;pila[1]=43;pila[10]=2;break;
     case 88: pila[0]=1051;pila[1]=43;pila[10]=2;break;
     case 89: pila[0]=1052;pila[1]=43;pila[10]=2;break;
     case 90: pila[0]=601;pila[10]=1;break;
     case 91: pila[0]=1042;pila[10]=1;break;
     case 92: pila[0]=1043;pila[10]=1;break;
     case 93: pila[0]=1044;pila[10]=1;break;
     case 94: pila[0]=1045;pila[10]=1;break;
     case 95: pila[0]=1046;pila[10]=1;break;
     case 96: pila[0]=1047;pila[10]=1;break;
     case 97: pila[0]=1012;pila[1]=24;pila[10]=2;break;
     case 98: pila[0]=1023;pila[10]=1;break;
     case 99: pila[0]=1024;pila[10]=1;break;
     case 100: pila[0]=1020;pila[10]=1;break;
     case 101: pila[0]=1021;pila[10]=1;break;
     case 102: pila[0]=1022;pila[10]=1;break;
     case 103: pila[0]=1053;pila[1]=33;pila[2]=1054;pila[10]=3;break;
    }
return pila;
}

int Matriz_colum(QString token,int estado){
    QStringList myOptions;
    myOptions << "init_library" << "library" << "end_library"<< "global_cte"<< "end_global_cte"<< "global_var" << "end_global_var"<< "function"<< "end_function"
              << "class"<< "main" << "endclass"<< ","<< "int"<< "float"<< "char"<< "string"<< "bool" << "void"<<
              "local_var"<< "end_local_var"<< "if"<< "end_if"<< "elseif"<< "else"<< "while"<< "end_while"<< "for"<< "end_for"<<"return"<< "read"<< "write"
              << "||"<< "&&"<< "!"<< "="<< "=="<< "!="<< "<"<< "<="<< ">"<< ">="<< "+"<< "-"<< "*"<< "/"<< "%"<< "("<< ")"
               << ":"<< ";"<< "$"<< "to";
    QStringList myOptions2;


    switch(estado){

     case 102:return 23;break; // cteentera
     case 103:return 24;break; // ctereal
     case 104:return 20;break; // ctenotacion
     case 105:return 21;break; // ctecaracter
     case 106:return 22;break; // ctestring

    }

    switch(myOptions.indexOf(token)){
    case 0:
      return 0;break;
    case 1:
      return 1;break;
    case 2:
      return 2;break;
    case 3:
      return 3;break;
    case 4:
      return 4;break;
    case 5:
      return 5;break;
    case 6:
      return 6;break;
    case 7:
      return 7;break;
    case 8:
      return 8;break;
    case 9:
      return 9;break;
    case 10:
      return 10;break;
    case 11:
      return 11;break;
    case 12:
      return 13;break;
    case 13:
      return 14;break;
    case 14:
      return 15;break;
    case 15:
      return 16;break;
    case 16:
      return 17;break;
    case 17:
      return 18;break;
    case 18:
      return 19;break;
    case 19:
      return 25;break;
    case 20:
      return 26;break;
    case 21:
      return 27;break;
    case 22:
      return 28;break;
    case 23:
      return 29;break;
    case 24:
      return 30;break;
    case 25:
      return 31;break;
    case 26:
      return 32;break;
    case 27:
      return 33;break;
    case 28:
      return 34;break;
    case 29:
      return 35;break;
    case 30:
      return 36;break;
    case 31:
      return 37;break;
    case 32:
      return 38;break;
    case 33:
      return 39;break;
    case 34:
      return 40;break;
    case 35:
      return 41;break;
    case 36:
      return 42;break;
    case 37:
      return 43;break;
    case 38:
      return 44;break;
    case 39:
      return 45;break;
    case 40:
      return 46;break;
    case 41:
      return 47;break;
    case 42:
      return 48;break;
    case 43:
      return 49;break;
    case 44:
      return 50;break;
    case 45:
      return 51;break;
    case 46:
      return 52;break;
    case 47:
      return 53;break;
    case 48:
      return 54;break;
    case 49:
      return 55;break;
    case 50:
      return 56;break;
    case 51:
      return 57;break;
    case 52:
      return 61;break;
    default:
        if( estado ==101 || estado == 100){
            return 12;
        }
    }
}
int colum(QString token,int estado){
    QStringList myOptions;
    myOptions << "init_library" << "library" << "end_library"<< "global_cte"<< "end_global_cte"<< "global_var" << "end_global_var"<< "function"<< "end_function"<< "class"<< "main" << "endclass"<< ","<< "int"<< "float"<< "char"<< "string"<< "bool" << "void"<< "ctenotacion"<< "ctecaracter"<< "ctestring"
    <<"cteentera"<<"ctereal"<< "local_var"<< "end_local_var"<< "if"<< "end_if"<< "elseif"<< "else"<< "while"<< "end_while"<< "for"<< "end_for"<<"return"<< "read"<< "write"<< "||"<< "&&"<< "!"<< "="<< "=="<< "!="<< "<"<< "<="<< ">"<< ">="<< "+"<< "-"<< "*"<< "/"<< "%"<< "("<< ")"
      << ":"<< ";"<< "$"<< "to";


    switch(estado){
      case 102: return 1023;break; //cteentera
      case 103: return 1024;break; //ctereal
      case 104: return 1020;break; //ctenotacion
      case 105: return 1021;break; //ctecaracter
      case 106: return 1022;break; //ctestring

    }
    switch(myOptions.indexOf(token)){
    case 0:
      return 1000;break;
    case 1:
      return 1001;break;
    case 2:
      return 1002;break;
    case 3:
      return 1003;break;
    case 4:
      return 1004;break;
    case 5:
      return 1005;break;
    case 6:
      return 1006;break;
    case 7:
      return 1007;break;
    case 8:
      return 1008;break;
    case 9:
      return 1009;break;
    case 10:
      return 1010;break;
    case 11:
      return 1011;break;
    case 12:
      return 1013;break;
    case 13:
      return 1014;break;
    case 14:
      return 1015;break;
    case 15:
      return 1016;break;
    case 16:
      return 1017;break;
    case 17:
      return 1018;break;
    case 18:
      return 1019;break;
    case 19:
      return 1020;break;
    case 20:
      return 1021;break;
    case 21:
      return 1022;break;
    case 22:
      return 1023;break;
    case 23:
      return 1024;break;
    case 24:
      return 1025;break;
    case 25:
      return 1026;break;
    case 26:
      return 1027;break;
    case 27:
      return 1028;break;
    case 28:
      return 1029;break;
    case 29:
      return 1030;break;
    case 30:
      return 1031;break;
    case 31:
      return 1032;break;
    case 32:
      return 1033;break;
    case 33:
      return 1034;break;
    case 34:
      return 1035;break;
    case 35:
      return 1036;break;
    case 36:
      return 1037;break;
    case 37:
      return 1038;break;
    case 38:
      return 1039;break;
    case 39:
      return 1040;break;
    case 40:
      return 1041;break;
    case 41:
      return 1042;break;
    case 42:
      return 1043;break;
    case 43:
      return 1044;break;
    case 44:
      return 1045;break;
    case 45:
      return 1046;break;
    case 46:
      return 1047;break;
    case 47:
      return 1048;break;
    case 48:
      return 1049;break;
    case 49:
      return 1050;break;
    case 50:
      return 1051;break;
    case 51:
      return 1052;break;
    case 52:
      return 1053;break;
    case 53:
      return 1054;break;
    case 54:
      return 1055;break;
    case 55:
      return 1056;break;
    case 56:
      return 1057;break;
    case 57:
      return 1061;break;
     default:
        if( estado ==101 || estado == 100){
            return 1012;
        }
    }
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
    case 105:return "Estado 105: " + caracteres +" Constante caracter" +"\r\n" ;//cout<<caracteres <<" Caracter"<<endl;break;
    case 106:return "Estado 106: " + caracteres +" Constante string" +"\r\n" ;//cout<<caracteres <<" Cadena"<<endl;break;
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



