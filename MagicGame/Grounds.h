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