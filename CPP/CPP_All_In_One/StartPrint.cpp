#include <iostream>
#include "StartPrint.h"
using namespace std;

// Update, Render 함수에서 객체 삭제 후 처리 수정해야됨

void OneStar::Update()
{
	this->_x += 1;

	if (this->_x >= 74)
	{
		this->_release = true;
	}
}

void OneStar::Render()
{
	if (this->_release == true)
	{
		return;
	}

	for (auto i = 0; i < this->_x; ++i)
	{
		cout << " ";
	}

	cout << "*";
}

//-----------------------------------

void TwoStar::Update()
{
	this->_x += 2;

	if (this->_x >= 73)
	{
		this->_release = true;
	}
}

void TwoStar::Render()
{
	if (this->_release == true)
	{
		return;
	}

	for (auto i = 0; i < this->_x; ++i)
	{
		cout << " ";
	}

	cout << "**";
}

//-----------------------------------

void ThreeStar::Update()
{
	this->_x += 3;

	if (this->_x >= 72)
	{
		this->_release = true;
	}
}

void ThreeStar::Render()
{
	if (this->_release == true)
	{
		return;
	}

	for (auto i = 0; i < this->_x; ++i)
	{
		cout << " ";
	}

	cout << "***";
}
