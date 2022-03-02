/****************************************************************
 *   Copyright (C) 2014 Adrien Combourieu                       *
 *                                                              *
 ****************************************************************/


#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//NOTE: including C functions into C++
extern "C" {

#include "../src/map.h"
#include "../src/mapinit.h"
#include "../src/outputstream.h"
#include "../src/mapapi.h"
#include "../src/freedata.h"

}


int main(int argc, char **argv)
{

  ///////// INITIALIZE with empty objects////////////
  MAP_InitInputType_t* _init_type = NULL;
  MAP_InitOutputType_t* _io_type = NULL;
  MAP_InputType_t* _u_type = NULL;
  MAP_ParameterType_t* _p_type = NULL;
  MAP_ConstraintStateType_t* _z_type = NULL;
  MAP_ContinuousStateType_t* _x_type = NULL;
  MAP_OutputType_t* _y_type = NULL;
  MAP_OtherStateType_t* _other_type = NULL;

  MAP_ERROR_CODE success = MAP_SAFE;
  char map_msg[MAP_ERROR_STRING_LENGTH] = "\0";
  // char map_msg[255] = "\0";
  MAP_ERROR_CODE ierr = MAP_SAFE;

  ///////// ALLOCATE MEMORY ///////////////
  cout << "--------- Start of MEMORY ALLOCATION ----------" << endl;

  _init_type = (MAP_InitInputType_t*)(uintptr_t)map_create_init_type(map_msg, &ierr); // @todo: check ierr==MAP_SAFE error for all
  _io_type = (MAP_InitOutputType_t*)(uintptr_t)map_create_initout_type(map_msg, &ierr);
  _u_type = (MAP_InputType_t*)(uintptr_t)map_create_input_type(map_msg, &ierr);
  _p_type = (MAP_ParameterType_t*)(uintptr_t)map_create_parameter_type(map_msg, &ierr);
  _z_type = (MAP_ConstraintStateType_t*)(uintptr_t)map_create_constraint_type(map_msg, &ierr);
  _x_type = (MAP_ContinuousStateType_t*)(uintptr_t)map_create_continuous_type(map_msg, &ierr);
  _y_type = (MAP_OutputType_t*)(uintptr_t)map_create_output_type(map_msg, &ierr);
  _other_type = (MAP_OtherStateType_t*)(uintptr_t)map_create_other_type(map_msg, &ierr);

  cout << "--------- End of MEMORY ALLOCATION ----------" << endl;

  /////////// INIATILIZE THE OBJECTS WITH USER INPUT //////////
  cout << "--------- Start of INITIALIZATION ----------" << endl;
  ////// SET ENVIRONMENT PARAMETERS //////
  double depth = 200.0;
  double g = 9.81;
  double rho = 1000.0;

  map_initialize_msqs_base(_u_type, _p_type, _x_type, _z_type, _other_type, _y_type, _io_type);
  map_set_sea_depth(_p_type, depth);
  map_set_gravity(_p_type, g);
  map_set_sea_density(_p_type, rho);


  ////// INPUT FILE PARSER (emulated here) /////////
  char line_def[1][255];
  char node_def[9][255];
  char element_def[9][255];
  char option_def[7][255];

  strcpy(line_def[0], "chainup  0.266 435.2 6.4E9 1.0 1.0E8 1.0 1.4 0.64\0");

  strcpy(node_def[0], "1  fix   638.35  0  -200  0 0 0 0 0\0");
  strcpy(node_def[1], "2  fix   -320      554.3    -200 0 0 0 0 0\0");
  strcpy(node_def[2], "3  fix   -320     -554.3    -200 0 0 0 0 0\0");
  strcpy(node_def[3], "4  Connect   39.4     0      -46.6   2.4E+4 0 0 0 0 0\0");
  strcpy(node_def[4], "5  Connect   -19.5     34.0    -46.6   2.4E+4 0 0 0 0\0");
  strcpy(node_def[5], "6  Connect   -19.5    -34.0    -46.6   2.4E+4  0 0 0 0\0");
  strcpy(node_def[6], "7  Vessel   5.22     -9.04    -13.4  0 0 0 0 0\0");
  strcpy(node_def[7], "8  Vessel   5.22      9.04     -13.4 0 0 0 0 0\0");
  strcpy(node_def[8], "9  Vessel   -10.44      0.0       -13.4 0 0 0 0 0\0");

  //   strcpy(element_def[0], "1 chainup  100 1 3 gy_pos h_fair\0");
  //   strcpy(element_def[1], "2 chainup2  100 2 4 gy_pos h_fair\0");
  strcpy(element_def[0], "1 chainup  637.0  1 4 tension_fair tension_anch\0");
  strcpy(element_def[1], "2 chainup  638.7 2 5 tension_fair tension_anch\0");
  strcpy(element_def[2], "3 chainup  638.7 3 6 tension_fair tension_anch\0");
  strcpy(element_def[3], "4 chainup  48 4 7 tension_fair tension_anch\0");
  strcpy(element_def[4], "5 chainup  48 4 8 tension_fair tension_anch\0");
  strcpy(element_def[5], "6 chainup  48 5 8 tension_fair tension_anch\0");
  strcpy(element_def[6], "7 chainup  48 5 9 tension_fair tension_anch\0");
  strcpy(element_def[7], "8 chainup  48 6 9 tension_fair tension_anch\0");
  strcpy(element_def[8], "9 chainup  48 6 7 tension_fair tension_anch\0");

  strcpy(option_def[0], "inner_ftol 1e-6\0");
  strcpy(option_def[1], "inner_gtol 1e-6\0");
  strcpy(option_def[2], "inner_xtol 1e-6\0");
  strcpy(option_def[3], "outer_tol 1e-3\0");
  strcpy(option_def[4], "inner_max_its 500\0");
  strcpy(option_def[5], "outer_max_its 1000\0");
  //strcpy(option_def[6], "repeat 12 280\0");

  /* set cable library data */
  strcpy(_init_type->library_input_str, line_def[0]); map_add_cable_library_input_text(_init_type);

  /* set node data */
  strcpy(_init_type->node_input_str, node_def[0]); map_add_node_input_text(_init_type);
  strcpy(_init_type->node_input_str, node_def[1]); map_add_node_input_text(_init_type);
  strcpy(_init_type->node_input_str, node_def[2]); map_add_node_input_text(_init_type);
  strcpy(_init_type->node_input_str, node_def[3]); map_add_node_input_text(_init_type);
  strcpy(_init_type->node_input_str, node_def[4]); map_add_node_input_text(_init_type);
  strcpy(_init_type->node_input_str, node_def[5]); map_add_node_input_text(_init_type);
  strcpy(_init_type->node_input_str, node_def[6]); map_add_node_input_text(_init_type);
  strcpy(_init_type->node_input_str, node_def[7]); map_add_node_input_text(_init_type);
  strcpy(_init_type->node_input_str, node_def[8]); map_add_node_input_text(_init_type);

  /* set line properties */
  strcpy(_init_type->line_input_str, element_def[0]); map_add_line_input_text(_init_type);
  strcpy(_init_type->line_input_str, element_def[1]); map_add_line_input_text(_init_type);
  strcpy(_init_type->line_input_str, element_def[2]); map_add_line_input_text(_init_type);
  strcpy(_init_type->line_input_str, element_def[3]); map_add_line_input_text(_init_type);
  strcpy(_init_type->line_input_str, element_def[4]); map_add_line_input_text(_init_type);
  strcpy(_init_type->line_input_str, element_def[5]); map_add_line_input_text(_init_type);
  strcpy(_init_type->line_input_str, element_def[6]); map_add_line_input_text(_init_type);
  strcpy(_init_type->line_input_str, element_def[7]); map_add_line_input_text(_init_type);
  strcpy(_init_type->line_input_str, element_def[8]); map_add_line_input_text(_init_type);

  /* set solver options */
  strcpy(_init_type->option_input_str, option_def[0]); map_add_options_input_text(_init_type);
  strcpy(_init_type->option_input_str, option_def[1]); map_add_options_input_text(_init_type);
  strcpy(_init_type->option_input_str, option_def[2]); map_add_options_input_text(_init_type);
  strcpy(_init_type->option_input_str, option_def[3]); map_add_options_input_text(_init_type);
  strcpy(_init_type->option_input_str, option_def[4]); map_add_options_input_text(_init_type);
  strcpy(_init_type->option_input_str, option_def[5]); map_add_options_input_text(_init_type);
  //strcpy(_init_type->option_input_str, option_def[6]); map_add_options_input_text(_init_type);

  strcpy(_init_type->summary_file_name,"baseline.sum.map\0"); map_set_summary_file_name(_init_type, map_msg, &ierr);



  /////////// CALL MAP_INIT //////////////////
  //WARNING: adrien@marco : map_init can fail without raising warning or exception and program continues: is it expected ?
  map_init(_init_type, _u_type, _p_type, _x_type, NULL, _z_type, _other_type, _y_type, _io_type, &ierr, map_msg);
  if (ierr!=MAP_SAFE) {
    std::cout << map_msg << std::endl;
  };


   //////////// DEALLOCATE SOME MEMORY NEEDED ONLY FOR INIT HERE //////////////
   MAPFREE(_init_type);
   MAPFREE(_io_type);

   cout << "--------- End of INITIALIZATION ----------" << endl;


   ///////// TIME SOLVER EMULATOR (forced displacements along X) /////////////

   cout << "--------- Start of SIMULATION ----------" << endl;

   int ntimesteps = 15000;
   double dt = 0.001;
   int num_points=100;
  ofstream position;
  ofstream tension;
  ifstream foamStarfile;
  int numRows=ntimesteps;int numCol=7;
  int temp=0;
  double fsdata[numRows][numCol];
 int Scale=40;
//  for (int i = 0; i < numRows; ++i) { fsdata[i].resize(numCol);}; // COl only time and motions
 foamStarfile.open("cylinder1.dat");
 if (!foamStarfile)
    {
        cout << "File opening failed.\n";
        return 1;
    }

  for (int i = 0; i < numRows; ++i) {
          for (int j = 0; j < numCol; ++j) {
          foamStarfile >> fsdata[i][j];}}
foamStarfile.close();

  char filename[16]; // For nodal positions
  char Tensionfile[16]="Lines.out";
  char Linename[16];
  int numline=9;
  char node[16];
  //double fsdata[7];
  double Tx; double Ty; double Tz;
  double* Txx=&Tx; double* Tyy=&Ty; double* Tzz=&Tz;
   double Surge; double Sway; double Heave; double Roll; double Pitch; double Yaw;

   Domain* data = (Domain*)_other_type->object;
   Vessel* vessel = &data->vessel;
   int N = _u_type->x_Len;

   for (int k = 0; k<ntimesteps; ++k) {

      cout << "Time step: " << k << endl;

Surge=fsdata[k][1]*Scale;Sway=fsdata[k][2]*Scale;Heave=fsdata[k][3]*Scale;Roll=fsdata[k][4];Pitch=fsdata[k][5];Yaw=fsdata[k][6];
//cout<<Surge<<endl<<Sway<<endl<<Heave<<endl<<Roll<<endl<<Pitch<<endl<<Yaw<<endl;


     map_offset_vessel(_other_type, _u_type, Surge, Sway, Heave, Roll, Pitch, Yaw, map_msg, &ierr);
     map_update_states(dt*(k+1), dt, _u_type, _p_type, _x_type, NULL, _z_type, _other_type, &ierr, map_msg);
     map_calc_output(dt*(k+1),_u_type, _p_type, _x_type, NULL, _z_type, _other_type,_y_type, &ierr, map_msg);

  //   cout << "Element 0 fairlead force: " << _y_type->Fx[0] << " " << _y_type->Fy[0] << " " << _y_type->Fz[0] << endl;

/* Writing the Tension output */



 // For first time opening the file
 if(k==0){

   tension.open(Tensionfile); // opens the file
   if( !tension ) { // file couldn't be opened
       cerr << "Error: file could not be opened" << endl;
       exit(1);
    }
    tension << "Time" << "\t ";
		for (int l=0; l<numline; l++){
    sprintf( Linename, "Line%d", l+1 );
    tension << Linename<<"Tx"<< "\t "<< Linename<<"Ty"<< "\t "<< Linename<<"Tz"<< "\t " ;}
		tension << "\n";
    tension << "\t"<<"[s]"<<"\t";
    for (int l=0; l<numline; l++){
      tension << "\t "<< "\t" <<"[N]"<< "\t "<< "\t" <<"[N]"<< "\t "<< "\t" <<"[N]"<< "\t"<<"\t";}
     tension<<"\n";
    tension << dt*(k+1)<<"\t";
    for (int nline= 0; nline<numline; ++nline){
      map_get_fairlead_force_3d( Txx,Tyy,Tzz, _other_type, nline, map_msg, &ierr);
       tension<<*Txx<<"\t"<<*Tyy<<"\t"<<*Tzz<<"\t";}
      tension<<"\n";
      tension.close();
		}
    else {
         tension.open(Tensionfile,ios::app); // opens the file
         if( !tension ) { // file couldn't be opened
          cerr << "Error: file could not be opened" << endl;
          exit(1);}
          tension << dt*(k+1)<<"\t";
          for (int nline= 0; nline<numline; ++nline){
            map_get_fairlead_force_3d( Txx,Tyy,Tzz, _other_type, nline, map_msg, &ierr);
             tension<<*Txx<<"\t"<<*Tyy<<"\t"<<*Tzz<<"\t";}
            tension<<"\n";
      }

/* Writing the line position for every time step */
for (int nline= 1; nline<numline+1; ++nline){
      sprintf( filename, "Line%d.dat", nline );



   if(k==0){
                position.open(filename); // opens the file
               if( !position ) { // file couldn't be opened
                        cerr << "Error: file could not be opened" << endl;
                        exit(1);
                               }
                position << "Time" << "\t ";
               for (int l=0; l<num_points; l++){
                sprintf( node, "Node%d", l+1 );
                position << node <<'x'<< "\t" << node <<'y'<< "\t " << node <<'z'<< "\t " ;}
               position << "\n";
               position.flush();
           }
// Opening the file to add the next nodal positions
  else {
            position.open(filename,ios::app);
            if( !position ) { // file couldn't be opened
                   cerr << "Error: file could not be opened" << endl;
            exit(1);}

        }




   // cout<<"Reading Line number "<<k<<endl;
   //cout<<"The file Being Written : "<<filename<<endl;
    double*  x= map_plot_x_array(_other_type, nline-1, num_points, map_msg, &ierr );
    double*  y= map_plot_y_array(_other_type, nline-1, num_points, map_msg, &ierr );
    double*  z= map_plot_z_array(_other_type, nline-1, num_points, map_msg, &ierr );

       position.flush();
       position << dt*(k+1) << "\t" ;
   for (int i=0; i<num_points; ++i)
      position << x[i] << "\t" << y[i] << "\t" << z[i]<< "\t";
      position<<"\n";
      position.close();
  }
  position.close();
  tension.close();
}
   cout << "--------- End of SIMULATION ----------" << endl;

   //////////////////// MEMORY DEALLOCATION /////////////////
   cout << "--------- Start of DEALLOCATION ----------" << endl;

   // MAP_ERROR_CODE success = MAP_SAFE;
   map_end(_u_type, _p_type, _x_type, NULL, _z_type, _other_type, _y_type, &ierr, map_msg);
   success = map_free_types(_u_type, _p_type, _x_type, _z_type, _other_type, _y_type);
   MAPFREE(_other_type);
   MAPFREE(_y_type);
   MAPFREE(_u_type);
   MAPFREE(_p_type);
   MAPFREE(_z_type);
   MAPFREE(_x_type);

  cout << "--------- End of DEALLOCATION ----------" << endl;

  /////////////////////////////////////////////////////////////

  cout << "C++ driver test ends" << endl;

  return(0);

}
