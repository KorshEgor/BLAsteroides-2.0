
#ifndef _MYMATH_LIB  // флаг подключения
#define _MYMATH_LIB

class Matr{
public:
	int w, h;
	float matr[100][100];
	void init(int W, int H){
		w = W; h = H;
		null();
		if(W == H){
			for( int u = 0; u < W; u++){
				matr[u][u] = 1;
			}
		}
	}
	void null(){
		for(int j = 0; j < w; j++){
			for(int u = 0; u < h; u++){
				matr[j][u] = 0;
			}
		}
	}
	void destroy(){

	};
	Matr operator+(Matr m)
	{
		Matr ret;
		if(w != m.w || h != m.h){
			ret.init(1,1);
			ret.matr[0][0] = -1;
		}else{
			ret.init(w,h);
			for(int j = 0; j < w; j++){
				for(int u = 0; u < h; u++){
					ret.matr[j][u] = matr[j][u] + m.matr[j][u];
				}
			}
		}
		return ret;
	}
	Matr operator*(float i)
	{
		Matr ret;
		ret.init(w,h);
		for(int j = 0; j < w; j++){
			for(int u = 0; u < h; u++){
				ret.matr[j][u] = matr[j][u] * i;
			}
		}
		return ret;
	}
	Matr operator*(Matr m)
	{
		Matr ret;
		if(m.h != w){
			ret.init(1,1);
			ret.matr[0][0] = -1;
		}else{
			ret.init(m.w, h);
			ret.null();
			for(int j = 0; j < h; j++){
				for(int u = 0; u < m.w; u++){
					for(int o = 0; o < w; o++){
						ret.matr[u][j] += matr[o][j]*m.matr[u][o];
					}
				}
			}
		}
		return ret;
	}
	Matr AlDop(){
		Matr m = Minor();
		if(!(m.matr[0][0] == -1 && w == 1 && h == 1)){
			if(m.w == m.h){
				for(int x = 0; x < m.w; x++){
					for(int y = 0; y < m.h; y++){
						if((x+y)%2) m.matr[x][y]*=-1;
					}
				}
			}
		}
		return m;
	}
	Matr Trans(){
		Matr m; m.init(h,w);
		for(int x = 0; x < w; x++){
			for(int y = 0; y < h; y++){
				m.matr[y][x] = matr[x][y];
			}
		}
		return m;
	}
	float Opred(){
		float ret = -1;
		int znak = 1;
		if(w == h){
			ret = 0;
			if(w == 1)
				return matr[0][0];
			if(w == 2)
				return matr[0][0]*matr[1][1] - matr[1][0]*matr[0][1];
			for(int x = 0; x < w; x++){
				Matr m; m.init(w-1,h-1);
				for(int u = 0; u < h-1; u++){
					for(int k = 0; k < w-1; k++){
						m.matr[k][u] = matr[k+(k >= x)][u+1];
					}
				}
				ret += matr[x][0]*m.Opred()*znak;
				znak*=-1;
			}
		}
		return ret;
	}
	Matr Minor(){
		Matr m, m1;
		if(w == h){
			m.init(w,h);
			for(int x = 0; x < w; x++){
				for(int y = 0; y < h; y++){
					m1.init(w-1,h-1);
					for(int u = 0; u < h-1; u++){
						for(int k = 0; k < w-1; k++){
							m1.matr[k][u] = matr[k+(k >= x)][u+(u >= y)];
						}
					}
					m.matr[x][y] = m1.Opred();
				}
			}
		}else{m.init(1,1); m.matr[0][0] = -1;}
		return m;
	}
	Matr Obr(){
		Matr m;
		float opr = Opred();
		if(w == h && opr != 0){
			m = AlDop().Trans()*(1/opr);
		}else{m.init(1,1); m.matr[0][0] = -1;}
		return m;
	}
};

#endif