#!/bin/bash

# Script para compilar y ejecutar la versión MPI de mTASEP (Julio)

# 1. Rutas
ROOT_DIR="/home/rodolfo/Descargas/mTASEP_CUDA"
BINARY_PATH="$ROOT_DIR/private/julio/mTASEP/bin/julio_demo_mTASEP_LK"

# 2. Verificar si se solicita compilación
do_build=false
if [[ "$1" == "--build" ]] || [[ ! -f "$BINARY_PATH" ]]; then
    do_build=true
fi

if [ "$do_build" = true ]; then
    echo "Iniciando proceso de compilación..."
    cd "$ROOT_DIR"
    ./autogen.sh -c ./configs/advanced/mpi -d 0
    cd build
    make julio_demo_mTASEP_LK
else
    echo "Saltando compilación (el binario ya existe). Usa ./run_mpi.sh --build para recompilar."
fi

# 4. Preparar carpeta de resultados
cd /home/rodolfo/Descargas/mTASEP_CUDA/private/rodolfo/mTASEP/
rm -rf RESLT_MPI
mkdir -p RESLT_MPI

# 5. Ejecutar con MPI (ajusta -n al número de núcleos deseado, ej: 4 u 8)
echo "Ejecutando simulación MPI..."
mpiexec -n 8 /home/rodolfo/Descargas/mTASEP_CUDA/private/julio/mTASEP/bin/julio_demo_mTASEP_LK \
--N 3 --L 101 --alpha_min 0.0 --alpha_max 1 \
--alpha_step 0.1 --alpha_n_points 11 --beta_min 0.0 --beta_max 1 \
--beta_step 0.1 --beta_n_points 11 --rho_min 1 --rho_max 1 --rho_step 0 \
--rho_n_points 1 --omega_in_min 0.0 --omega_in_max 1 --omega_in_step 0.1 \
--omega_in_n_points 11 --omega_out_min 0.0 --omega_out_max 1 \
--omega_out_step 0.1 --omega_out_n_points 11 --lateral_movement 1 \
--max_experiments 10 --max_simulations_per_experiment 101 \
--simulation_step_to_start_gathering_data 95 \
--output_space_state_diagram 0 --output_microtubule_state 1 \
--root_output_folder RESLT_MPI
