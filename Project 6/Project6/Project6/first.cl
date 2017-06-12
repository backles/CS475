kernel
void All(global const float *dA, global const float *dB, global float *dC, global const int *option, local float *result){
	int gid = get_global_id( 0 );
	int threadNumber = get_local_id(0);
	int workGroupNumber = get_group_id(0);
	int numberOfItems = get_local_size(0);
	switch( (intptr_t)option){
		case 0: 
			dC[gid] = dA[gid] * dB[gid];
			break;
		case 1:
			dC[gid] = dA[gid] * dB[gid] + dC[gid];
			break;
		case 2:
			result[threadNumber] = dA[gid] * dB[gid];
			for(int offset =1; offset < numberOfItems; offset *=2){
				int mask = 2*offset -1;
				barrier(CLK_LOCAL_MEM_FENCE);
				if(threadNumber == 0 && mask == 0){
					result[threadNumber] = result[threadNumber] + result[threadNumber+offset];
				}
			}
			barrier(CLK_LOCAL_MEM_FENCE);
			if(threadNumber == 0){
				dC[workGroupNumber] = result[0];
			}
		break;
	}
}