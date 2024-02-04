#pragma once
#include"Pch.h"
#include"DebugTools/Commands/ICommand.h"

class DebugTools
{
public:
	
	~DebugTools() {};

	static DebugTools* GetInstance();

	/// <summary>
	/// �R�}���h��ǉ�����
	/// </summary>
	static void addCommand(ICommand* command,const string &log=0);
	
	/// <summary>
    /// �o�^�����R�}���h����ׂď��� 
	/// </summary>
	static void ClearCommand();

	/// <summary>
	/// DebugTool��g���ꍇ��Scene��Update�̍Ō�ɕK���͂���
	/// </summary>
	static ViewProjection ConvertViewProjection(ViewProjection& viewProjection);

	/// <summary>
	/// �o�^�����R�}���h�̍X�V����
	/// </summary>
	static void UpdateExecute(int commandNumber);

	/// <summary>
	/// �o�^�����R�}���h�̕`�揈��
	/// </summary>
	static void DrawExecute(int commandNumber);

	/// <summary>
	/// gameScene�̓�ł͎g��Ȃ�
	/// </summary>
	static void SetViewProjection(ViewProjection viewProjection) { DebugTools::GetInstance()->viewProjection_ = viewProjection; }

	/// <summary>
	/// gameScene�̓�ł͎g��Ȃ�
	/// </summary>
	static void SetIsviewProjectionReWrite(bool Flag) { DebugTools::GetInstance()->isViewProjectionReWrite = Flag; }

	/// <summary>
	/// gameScene�̓�ł͎g��Ȃ�
	/// </summary>
	static ViewProjection GetViewProjection() { return DebugTools::GetInstance()->viewProjection_; }

private:

	vector<ICommand*> commands;

	ViewProjection viewProjection_{};
	ViewProjection SaveViewProjection_{};
	bool isViewProjectionReWrite=false;
	bool isCameraSave = false;
};
