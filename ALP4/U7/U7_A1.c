#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "timer.h"

#define DIM 2  /* Two-dimensional system */
#define X 0    
#define Y 1    

const double G = 6.673e-11;  

// define vector
typedef double vect_t[DIM];  

struct particle_s {
   double m;  // mass
   vect_t s;  // position
   vect_t v;  
};

void usage(char* prog_name) {
   printf("usage: %s <number of particles> <number of timesteps>\n", prog_name);
   printf("   <size of timestep> <output frequency>\n");
   printf("   <g|i>\n");
   printf("   'g': program should generate init conds\n");
   printf("   'i': program should get init conds from stdin\n");
    
   exit(-1);
}

void get_args(int argc, char* argv[], int* n_p, int* n_steps_p, double* delta_t_p, int* output_freq_p, char* g_i_p) {
   if (argc != 6){
      usage(argv[0]);
   }

   *n_p = strtol(argv[1], NULL, 10);
   *n_steps_p = strtol(argv[2], NULL, 10);
   *delta_t_p = strtod(argv[3], NULL);
   *output_freq_p = strtol(argv[4], NULL, 10);
   *g_i_p = argv[5][0];

   // handle inputs
   if (*n_p <= 0 || *n_steps_p < 0 || *delta_t_p <= 0 || (*g_i_p != 'g' && *g_i_p != 'i')){
      usage(argv[0]);
   }
}

 // get mass, position and velocity
void get_init_cond(struct particle_s curr[], int n) {
   printf("mass, x-coord, y-coord, x-velocity, y-velocity?\n");

   for (int part = 0; part < n; part ++) {
      scanf("%lf", &curr[part].m);
      scanf("%lf", &curr[part].s[X]);
      scanf("%lf", &curr[part].s[Y]);
      scanf("%lf", &curr[part].v[X]);
      scanf("%lf", &curr[part].v[Y]);
   }
}

// default initialize, put all particles on the x axis and give speed with y axis
void gen_init_cond(struct particle_s curr[], int n) {
   // referenced to earth
   double mass = 6.0e24;
   double gap = 1.0e5;
   double speed = 10.0e5;

   for (int part = 0; part < n; part ++) {
      curr[part].m = mass;
      curr[part].s[X] = part * gap;
      curr[part].s[Y] = 0.0;
      curr[part].v[X] = 0.0;

      // to render different directions
      if (part % 2 == 0)
         curr[part].v[Y] = speed;
      else
         curr[part].v[Y] = -speed;
   }
} 

// output the state of particles
void output_state(double time, struct particle_s curr[], int n) {
   printf("%.2f\n", time);
   printf("       Postion x    Postion y   Speed x     Speed y\n");
   for (int part = 0; part < n; part++) {
      printf("%3d %10.3e ", part, curr[part].s[X]);
      printf("  %10.3e ", curr[part].s[Y]);
      printf("  %10.3e ", curr[part].v[X]);
      printf("  %10.3e\n", curr[part].v[Y]);
   }
   printf("\n");
} 


// compute the total force on particle part.
void compute_force(int part, vect_t forces[], struct particle_s curr[], int n) {
   double mg; 
   vect_t f_part_k;
   double len, fact;

   forces[part][X] = forces[part][Y] = 0.0;

   for (int k = 0; k < n; k ++) {
      if (k != part) {
         // distance between k and part
         f_part_k[X] = curr[part].s[X] - curr[k].s[X];
         f_part_k[Y] = curr[part].s[Y] - curr[k].s[Y];
         len = sqrt(pow(f_part_k[X], 2) + pow(f_part_k[Y], 2));

         mg = -G * curr[part].m * curr[k].m;

         // torque
         fact = mg / pow(len, 3);
         f_part_k[X] *= fact;
         f_part_k[Y] *= fact;
   
         forces[part][X] += f_part_k[X];
         forces[part][Y] += f_part_k[Y];
      }
   }
}


// update the information of 
void update_part(int part, vect_t forces[], struct particle_s curr[], int n, double delta_t) {
   // delta v = at = F/m * t
   double fact = delta_t / curr[part].m;

   curr[part].s[X] += delta_t * curr[part].v[X];
   curr[part].s[Y] += delta_t * curr[part].v[Y];
   curr[part].v[X] += fact * forces[part][X];
   curr[part].v[Y] += fact * forces[part][Y];
}

// compute the kinetic and potential energy in the system
void compute_energy(struct particle_s curr[], int n, double* kin_en_p, double* pot_en_p) {
   vect_t diff;
   double pe = 0.0, ke = 0.0;
   double dist, speed_sqr;

   // E = 1/2 mv2
   for (int i = 0; i < n; i++) {
      speed_sqr = pow(curr[i].v[X], 2) + pow(curr[i].v[Y], 2);
      ke += curr[i].m * speed_sqr;
   }
   ke *= 0.5;

   for (int i = 0; i < n-1; i++) {
      for (int j = i+1; j < n; j++) {
         diff[X] = curr[i].s[X] - curr[j].s[X];
         diff[Y] = curr[i].s[Y] - curr[j].s[Y];
         dist = sqrt(pow(diff[X], 2) + pow(diff[Y], 2));
         pe += -G * curr[i].m * curr[j].m / dist;
      }
   }

   *kin_en_p = ke;
   *pot_en_p = pe;
} 

int main(int argc, char* argv[]) {
   int n;                      // number of particles
   int n_steps;                // number of timesteps        
   int step;                   // current step               
   int part;                   // current particle           
   int output_freq;            // frequency of output      

   double delta_t;             // size of timestep           
   double t;                   // current Time         

   struct particle_s* curr;    // current state of system    
   vect_t* forces;             // forces on each particle    
   char g_i;                  

#  ifdef COMPUTE_ENERGY
   double kinetic_energy, potential_energy;
#  endif

   double start, finish;

   get_args(argc, argv, &n, &n_steps, &delta_t, &output_freq, &g_i);

   curr = malloc(n * sizeof(struct particle_s));
   forces = malloc(n * sizeof(vect_t));
   
   if (g_i == 'i'){
      get_init_cond(curr, n);
   }else{
      gen_init_cond(curr, n);
   }

   GET_TIME(start);

#  ifdef COMPUTE_ENERGY
   compute_energy(curr, n, &kinetic_energy, &potential_energy);
   printf("   PE = %e, KE = %e, Total Energy = %e\n",
         potential_energy, kinetic_energy, kinetic_energy+potential_energy);
#  endif

#  ifndef NO_OUTPUT
   output_state(0, curr, n);
#  endif

   for (step = 1; step <= n_steps; step++) {
      t = step * delta_t;

      for (part = 0; part < n; part++){
         compute_force(part, forces, curr, n);
      }

      // update the state of particle
      for (part = 0; part < n; part++){
         update_part(part, forces, curr, n, delta_t);
      }

#     ifdef COMPUTE_ENERGY
      compute_energy(curr, n, &kinetic_energy, &potential_energy);
      printf("   PE = %e, KE = %e, Total Energy = %e\n",
            potential_energy, kinetic_energy, kinetic_energy+potential_energy);
#     endif

#     ifndef NO_OUTPUT
      if (step % output_freq == 0)
         output_state(t, curr, n);
#     endif
   }
   
   GET_TIME(finish);

   printf("Passed time = %e seconds\n", finish - start);

   free(curr);
   free(forces);
   return 0;
}

