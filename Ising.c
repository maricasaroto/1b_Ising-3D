#include <stdio.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <time.h>




int main(int argc, char **argv){
const int Nx=2 ;
const int Ny=Nx;
const int Nz=Nx;
const int N=Nx*Ny*Nz;
const double Ti=1.0;
const double dT=0.1;
const double Tf=10.0;
const double MCS=1e6;
	int i, ju, jd, j, il,ir, k, kf, kb, p, p1;
	int *spin;
	double  M=0, M1, M2, M3, E=0, E1, E2, dE, T, cp, xm;
	const gsl_rng_type * t;
	gsl_rng * r;
	gsl_rng_env_setup();
	r = gsl_rng_alloc (gsl_rng_ranlxs0);
	gsl_rng_set (r,time(NULL));
	T=Ti;
    FILE *fp;
    char f_name[60];
    sprintf(f_name,"dat/output_%d.dat",Nx);
    fp = fopen (f_name, "w");
    spin=(int*)calloc((Nx*Ny*Nz), sizeof(int));
//Construção - Inicio
	for (i=0; i<N; i++){
			if (gsl_rng_uniform (r)<.5){
				spin[i]=-1;
			}else{
				spin[i]=1;
			}
	}
//Construção - FIM
//simulação - inicio
	for(T=Ti; T<Tf; T+=dT){
		//transiente - inicio
		for (p=0; p<1000*(N); p++){
			i=abs(gsl_rng_uniform (r)*Nx);
			j=abs(gsl_rng_uniform (r)*Ny);
			k=abs(gsl_rng_uniform (r)*Nz);
			ir=(i+1)%Nx;
			il=(i+Nx-1)%Nx;
			ju=(j+1)%Ny;
			jd=(j-1+Ny)%Ny;
			kf=(k+1)%Nz;
			kb=(k-1+Nz)%Nz;
			dE=2*(spin[(i*Ny+j)*Nz+k]*(spin[(i*Ny+ju)*Nz+k]+spin[(i*Ny+jd)*Nz+k]+spin[(il*Ny+j)*Nz+k]
                                      +spin[(ir*Ny+j)*Nz+k]+spin[(i*Ny+j)*Nz+kf]+spin[(i*Ny+j)*Nz+kb]));
				if (gsl_rng_uniform (r)<(exp(-dE/T)))
					spin[(i*Ny+j)*Nz+k]=-1;
		}
		//transiente - fim
		//calculo en e mag - inicio
		E=0; M=0;
			for (i=0; i<Nx; i++){
				for(j=0; j<Ny; j++){
                    for(k=0; k<Nz; k++){
                        ir=(i+1)%Nx;
                        ju=(j+1)%Ny;
                        kf=(k+1)%Nz;
                        E-= spin[(i*Ny+j)*Nz+k]*(spin[(i*Ny+ju)*Nz+k]+spin[(ir*Ny+j)*Nz+k]+spin[(i*Ny+j)*Nz+kf]);
                        M+= spin[(i*Ny+j)*Nz+k];
                    }
                }
			}
		//calculo en e mag - fim
			E1=0; E2=0; M1=0; M2=0;
		for (p1=0; p1<MCS; p1++){
			for (p=0; p<(N); p++){
                i=abs(gsl_rng_uniform (r)*Nx);
                j=abs(gsl_rng_uniform (r)*Ny);
                k=abs(gsl_rng_uniform (r)*Nz);
                ir=(i+1)%Nx;
                il=(i+Nx-1)%Nx;
                ju=(j+1)%Ny;
                jd=(j-1+Ny)%Ny;
                kf=(k+1)%Nz;
                kb=(k-1+Nz)%Nz;
				dE=2*(spin[(i*Ny+j)*Nz+k]*(spin[(i*Ny+ju)*Nz+k]+spin[(i*Ny+jd)*Nz+k]+spin[(il*Ny+j)*Nz+k]
                                          +spin[(ir*Ny+j)*Nz+k]+spin[(i*Ny+j)*Nz+kf]+spin[(i*Ny+j)*Nz+kb]));
				if (gsl_rng_uniform (r)<(exp(-dE/T))){
						spin[(i*Ny+j)*Nz+k]*=-1;
						E+= dE;
						M+= 2*spin[(i*Ny+j)*Nz+k];
						}
				E1+= E; M1+= fabs(M);M3+= pow(M, 4); E2+= pow(E, 2); M2+= pow(M, 2);
			}
		}
		E1/= MCS*N; E2/= MCS*N; M3/= MCS*N; M1/= MCS*N; M2/= MCS*N;
		cp= (E2 - E1*E1)/(T*T); xm= (M2 - M1*M1)/(T);
 		E1/= N; M1/= N; M3/= N;  cp/= N; xm/= N; 
        fprintf (fp,"%e %e %e %e %e %e\n" , T, E1, fabs(M1), cp, xm, 1-M3/3*(M2*M2));
    }

fclose(fp);	
  gsl_rng_free (r);
	return 0;
}
