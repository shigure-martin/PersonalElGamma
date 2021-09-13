#include <iostream>
#include <time.h>

const int Mod = 29;

using namespace std;

struct Point{
	int x;
	int y;
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Point() {
		
	}
};

void exgcd(long a,long b, long& d, long& x, long& y){
    if(!b) { d = a; x = 1; y = 0; }
    else{ exgcd(b, a%b, d, y, x); y -= x*(a/b); }
}

long inv(long a, long p) {
    long d,x,y;
    exgcd(a,p,d,x,y);
    return d == 1 ? (x+p)%p : -1;
}

int Lamda(Point point_p, Point point_q, bool isNormal) {
	if(isNormal) {
		int result;
		result = ((point_q.y - point_p.y) * inv((point_q.x - point_p.x) % Mod, Mod)) % Mod;
		return result;
	} else {
		int result;
		result = ((3 * point_p.x * point_p.x) * inv((2 * point_p.y) % Mod, Mod)) % Mod;
		return result;
	}
}

Point generatePoint(Point point_p, Point point_q) {
	
	Point point;
	int lamda;
	if(point_p.x == point_q.x && point_p.y == point_q.y) {
		lamda = Lamda(point_p, point_q, false);
	} else {
		lamda = Lamda(point_p, point_q, true);
	}
	
	point.x = (lamda * lamda - point_p.x - point_q.x) % Mod;
	point.y = (lamda * (point_p.x - point.x) - point_p.y) % Mod;
	if(point.x < 0)
		point.x += Mod;
	if(point.y < 0)
		point.y += Mod;
	
	return point;
}

pair<Point, Point> pointEncrypt(Point point_k, Point* points, Point point_m) {
	srand(time(NULL));
	int r = rand() % 29;
	//cout << r << endl;
	
	pair<Point, Point> result;
	Point first_p, second_p;
	
	first_p.x = points[r].x;
	first_p.y = points[r].y;
	
	Point temp = generatePoint(points[r], point_k);
	
	second_p.x = temp.x + point_m.x;
	second_p.y = temp.y + point_m.y;
	
//	second_p = generatePoint(generatePoint(points[r], point_k), point_m);
	
	result.first = first_p;
	result.second = second_p;
	
	return result;
}

Point pointDecrypt(pair<Point, Point> C, int k, Point* points) {
	Point result;
	Point first, second;
	first = C.first;
	second = C.second;
	
	Point temp = generatePoint(first, points[k]);
	
	result.x = second.x - temp.x;
	result.y = second.y - temp.y;
	
	return result;
}

int main(char* argv, int argc){
	
	Point* G = new Point(13, 24);
	
	Point point[29];
	point[0] = *G;
	//cout << "所有的点为：" << endl; 
	for (int i = 1; i < Mod; i++) {
		point[i] = generatePoint(*G, point[i-1]);
		//cout << point[i].x << "," << point[i].y << endl;
	}
	
	cout << "请输入需要加密的点，x与y用空格隔开" << endl;
	int x, y;
	cin >> x >> y;
	Point* M = new Point(x, y);
	
	cout << "请输入私钥:" << endl;
	int k;
	cin >> k;
	k = k % Mod;
	cout << "生成公钥为:" << "(" << point[k].x << "," << point[k].y << ")" << endl;
	
	pair<Point, Point> C = pointEncrypt(point[k], point, *M);
	cout << "加密结果为:" << "(" << C.first.x << "," << C.first.y <<"),(" << C.second.x << "," << C.second.y << ")" << endl;
	
	Point M_e;
	M_e = pointDecrypt(C, k, point);
	cout << "解密结果为:" << "(" << M_e.x << "," << M_e.y << ")" << endl;
	
	return 0;
} 
