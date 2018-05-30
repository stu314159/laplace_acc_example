#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>

const int WIDTH = 1000;
const int HEIGHT = 1000;
const double TEMP_TOLERANCE = 0.01;

double Temperature[HEIGHT+2][WIDTH+2];
double Temperature_previous[HEIGHT+2][WIDTH+2];

void initialize();
void track_progress(int iter, double wdt);

int main(int argc, char * argv[])
{

  int iteration = 1;
  double worst_dt = 100;
  time_t start_time, stop_time;

  time(&start_time);
  initialize();
#pragma acc data copy(Temperature_previous), create(Temperature)
  {
	  while(worst_dt > TEMP_TOLERANCE) {
#pragma acc parallel loop collapse(2)
		  for(int i = 1; i<=HEIGHT; i++) {
			  for(int j = 1; j<=WIDTH; j++){
				  Temperature[i][j] = 0.25*(Temperature_previous[i+1][j]
																	  + Temperature_previous[i-1][j]
																								  + Temperature_previous[i][j+1]
																															+ Temperature_previous[i][j-1]);
			  }
		  }
		  worst_dt = 0.0;

		  for(int i = 1; i <= HEIGHT; i++){
			  for(int j = 1; j <= WIDTH; j++){
				  worst_dt = fmax(fabs(Temperature[i][j] -
						  Temperature_previous[i][j]),
						  worst_dt);
				  Temperature_previous[i][j] = Temperature[i][j];
			  }
		  }

		  if((iteration % 100) == 0) {
			  #pragma acc update host(Temperature)
			  track_progress(iteration, worst_dt);
		  }
		  iteration++;
	  }
  }
  time(&stop_time);
  //elapsed_time = difftime(stop_time,start_time);
  double e_time = (stop_time - start_time);
  std::cout << "Max error at iteration " << iteration-1 << " was " << worst_dt << std::endl;
  std::cout << "Total time was " << e_time << " seconds." << std::endl;

  return 0;
}

void initialize(){

  for(int i = 0; i <= HEIGHT+1; i++){
	  for(int j = 0; j <=  WIDTH+1; j++){
		  Temperature_previous[i][j] = 0.0;
	  }
  }

  for(int i = 0; i <= HEIGHT+1; i++){
	  Temperature_previous[i][0] = 0.0;
	  Temperature_previous[i][WIDTH+1] = (100./HEIGHT)*i;
  }

  for(int j = 0; j <= WIDTH+1; j++){
	  Temperature_previous[0][j]  = 0.0;
	  Temperature_previous[HEIGHT+1][j] = (100./WIDTH)*j;
  }

}

void track_progress(int iter, double worst_dt){
  std::cout << "------------ Iteration number: " << iter << " ---------" << std::endl;
  for(int i = HEIGHT-3; i <= HEIGHT; i++){
	  std::cout << "[" << i <<"," << i <<"]: " << Temperature[i][i] << "   ";
  }
  std::cout << " worst DT = " << worst_dt;
  std::cout << std::endl;
}




