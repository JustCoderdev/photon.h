#include <photon-runner.h>

Runner_Init_Func   pw_vp_to_fp_runner_init(void* func)
{
	return (Runner_Init_Func)func;
}

Runner_Loop_Func   pw_vp_to_fp_runner_loop(void* func)
{
	return (Runner_Loop_Func)func;
}

Runner_Deinit_Func pw_vp_to_fp_runner_deinit(void* func)
{
	return (Runner_Deinit_Func)func;
}

