#pragma once


//���� ������������
enum Grounds {
	None,		//������ (������(������ ���))
	Normal,		//������� ����� 
	NormalItem,	//��� ��������� ����� ����� ���� ������� �� � ����� ��� ���������������
	Ice,		//˸�  (�������� � ���������� �������� ����� �� ���� ����������� ���������)
	Water,		//���� (�������� � ���������� �������� ����� ��� �������)
	Lava		//���� (�������� � ���������� �������� ����� �� ���� ����������� ������)
};


enum Side {
	Up,
	Jump,
	Right,
	Left
};

//�����, �������� ����� �� ������ 
struct Point {
	int x;
	int y;

	explicit Point() :x(0), y(0) {}
	explicit Point(int x, int y) : x(x), y(y) {}

	bool operator==(Point point) {
		if (point.x == this->x && point.y == this->y)
			return true;
		return false;
	}
	bool operator!=(Point point) {
		if (!(*this == point))
			return true;
		return false;
	}

	bool operator<=(Point point) {
		if (x <= point.x && y <= point.y)
			return true;
		return false;
	}
	bool operator>=(Point point) {
		if (x >= point.y && y >= point.y)
			return true;
		return false;
	}
};