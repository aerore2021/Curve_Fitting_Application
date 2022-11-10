#include <iostream>
#include <fstream>
#include <string>
#include "DataInput.h"
#include "algorithm.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"


int num_number;
std::vector<double> num_X, num_Y;
bool show_inquire_L = 0;
bool show_inquire_D = 0;
bool show_inquire_C = 0;
bool show_inquire_P = 0;
static void PushStyleCompact()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.60f)));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.60f)));
}

static void PopStyleCompact()
{
	ImGui::PopStyleVar(2);
}

void input(void)
{

	ImGui::Begin("Data Input", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	static char char_x[MAX_INPUT_NUMBER * sizeof(double)] = "\0";
	static char char_y[MAX_INPUT_NUMBER * sizeof(double)] = "\0";
	static char char_num[MAX_INPUT_NUMBER * sizeof(int)] = "\0";
	static char char_num_erase[MAX_INPUT_NUMBER * sizeof(int)] = "\0";
	struct TextFilters {
		//如果是数字、小数点或者空格，返回0；其他返回1；
		static int FilterImGuiLetters(ImGuiInputTextCallbackData* data) {
			if (data->EventChar < 256 && strchr("1234567890. \n", (char)data->EventChar))
				return 0;
			return 1;
		}
	};
	enum Mode
	{
		Mode_Input,
		Mode_Modify,
		Mode_Erase
	};

	static int mode = 0;
	if (ImGui::RadioButton("input", mode == Mode_Input))
	{
		mode = Mode_Input;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("modify", mode == Mode_Modify))
	{
		mode = Mode_Modify;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("erase", mode == Mode_Erase))
	{
		mode = Mode_Erase;
	}
	if (mode == Mode_Input)
	{
		ImGui::SetNextItemWidth(INPUT_WIDTH_XANDFX);
		ImGui::InputText("x", char_x, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
		ImGui::SetNextItemWidth(INPUT_WIDTH_XANDFX);
		ImGui::InputText("f(x)", char_y, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
		
		if (ImGui::Button("OK") && strcmp(char_x, "\0") && strcmp(char_y, "\0"))
		{
			double num_x = std::stod(char_x);
			double num_fx = std::stod(char_y);
			num_X.push_back(num_x);
			num_Y.push_back(num_fx);
			memset(char_x, '\0', sizeof(char));
			memset(char_y, '\0', sizeof(char));
		}
		ImGui::Separator();
		ImGui::Text("Please don't press OK button with blank input!");
	}
	else if (mode == Mode_Modify)
	{
		ImGui::SameLine();
		ImGui::SetNextItemWidth(INPUT_WIDTH_NUMBER);
		ImGui::InputText("number", char_num, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
		ImGui::SetNextItemWidth(INPUT_WIDTH_XANDFX);
		ImGui::InputText("x", char_x, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
		ImGui::SetNextItemWidth(INPUT_WIDTH_XANDFX);
		ImGui::InputText("f(x)", char_y, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
		if (ImGui::Button("OK") && strcmp(char_x, "\0") && strcmp(char_y, "\0") && strcmp(char_num, "\0"))
		{
			
			double num_x = std::stod(char_x);
			double num_fx = std::stod(char_y);
			num_number = std::stoi(char_num);
			if (num_number < num_X.size())
			{
				num_X.insert(num_X.begin() + num_number, num_x);
				num_Y.insert(num_Y.begin() + num_number, num_fx);
				num_X.erase(num_X.begin() + num_number + 1);
				num_Y.erase(num_Y.begin() + num_number + 1);
				memset(char_x, '\0', sizeof(char));
				memset(char_y, '\0', sizeof(char));
			}
		}
		ImGui::Separator();
		ImGui::Text("Please don't press OK button with blank input!");
	}
	else if (mode == Mode_Erase)
	{
		ImGui::SetNextItemWidth(INPUT_WIDTH_XANDFX);
		ImGui::InputText("number", char_num_erase, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
		ImGui::Text("\n");
		if (ImGui::Button("OK") && strcmp(char_num_erase, "\0"))
		{

			num_number = std::stoi(char_num_erase);
			if (num_number < num_X.size())
			{
				num_X.erase(num_X.begin() + num_number);
				num_Y.erase(num_Y.begin() + num_number);
			}

		}
		ImGui::Separator();
		ImGui::Text("Please don't press OK button with blank input!");
	}
	
	ImGui::End();
}

void showdata(void)
{
	ImGui::Begin("Data", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;
	static int freeze_cols = 1;
	static int freeze_rows = 1;
	int max_col = 25;

	PushStyleCompact();
	ImGui::SetNextItemWidth(ImGui::GetFrameHeight());
	ImGui::SetNextItemWidth(ImGui::GetFrameHeight());
	PopStyleCompact();

	if (ImGui::BeginTable("##something", max_col, flags))
	{
		ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
		ImGui::TableSetupColumn("number", ImGuiTableColumnFlags_NoHide);
		for (int i = 0; i < max_col - 1; i++)
		{
			std::string s = (std::to_string(i));
			const char* t = s.c_str();
			ImGui::SetNextItemWidth(30);
			ImGui::TableSetupColumn(t);
		}

		ImGui::TableHeadersRow();

		for (int row = 0; row < 2; row++)
		{
			ImGui::TableNextRow();
			for (int column = 0; column < max_col; column++)
			{
				if (!ImGui::TableSetColumnIndex(column) && column > 0)
					continue;
				if (column == 0)
				{
					if (row == 0)
						ImGui::Text("x");
					else
						ImGui::Text("f(x)");
				}
				else
				{
					if (row == 0)
					{
						if (num_X.size() >= column)ImGui::Text((std::to_string(num_X[column - 1])).c_str());
						else ImGui::Text("");
					}
					else
					{
						if (num_Y.size() >= column)ImGui::Text((std::to_string(num_Y[column - 1])).c_str());
						else ImGui::Text("");
					}
				}
			}
		}
		ImGui::EndTable();
	}
	ImGui::End();
}
void inquire_L()
{
	static char char_x[MAX_INPUT_NUMBER * sizeof(double)] = "\0";
	extern int nofL;
	struct TextFilters {
		//如果是数字、小数点或者空格，返回0；其他返回1；
		static int FilterImGuiLetters(ImGuiInputTextCallbackData* data) {
			if (data->EventChar < 256 && strchr("1234567890. \n", (char)data->EventChar))
				return 0;
			return 1;
		}
	};
	ImGui::SetNextItemWidth(INPUT_WIDTH_XANDFX);
	ImGui::InputText("x", char_x, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
	ImGui::SameLine();
	if (ImGui::Button("OK") && strcmp(char_x, "\0") )
	{
		
		show_inquire_L = true;
	}
	if (show_inquire_L)
	{
		double num_x = std::stod(char_x);
		double y = L_interpolation(nofL, num_X, num_Y, num_x);
		ImGui::Text("\n\n");
		ImGui::Text("f(x) = "); ImGui::SameLine();
		ImGui::Text((std::to_string(y)).c_str());
	}
}
void inquire_D()
{
	static char char_x[MAX_INPUT_NUMBER * sizeof(double)] = "\0";
	extern int nofD;
	struct TextFilters {
		static int FilterImGuiLetters(ImGuiInputTextCallbackData* data) {
			if (data->EventChar < 256 && strchr("1234567890. \n", (char)data->EventChar))
				return 0;
			return 1;
		}
	};
	ImGui::SetNextItemWidth(INPUT_WIDTH_XANDFX);
	ImGui::InputText("x", char_x, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
	ImGui::SameLine();
	if (ImGui::Button("OK") && strcmp(char_x, "\0"))
	{

		show_inquire_D = true;
	}
	if (show_inquire_D)
	{
		double num_x = std::stod(char_x);
		double y = Divided_Difference(nofD, num_X, num_Y, num_x);
		ImGui::Text("\n\n");
		ImGui::Text("f(x) = "); ImGui::SameLine();
		ImGui::Text((std::to_string(y)).c_str());
	}
}

void inquire_C()
{
	static char char_x[MAX_INPUT_NUMBER * sizeof(double)] = "\0";
	std::vector<std::vector<double>> a = Create_Cubic_Spline_Interpolation(num_X, num_Y);
	struct TextFilters {
		static int FilterImGuiLetters(ImGuiInputTextCallbackData* data) {
			if (data->EventChar < 256 && strchr("1234567890. \n", (char)data->EventChar))
				return 0;
			return 1;
		}
	};
	ImGui::SetNextItemWidth(INPUT_WIDTH_XANDFX);
	ImGui::InputText("x", char_x, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
	ImGui::SameLine();
	if (ImGui::Button("OK") && strcmp(char_x, "\0"))
	{

		show_inquire_C = true;
	}
	if (show_inquire_C)
	{
		double num_x = std::stod(char_x);
		double y = Compute_Cubic_Spline(a, num_X, num_x);

		ImGui::Text("\n\n");
		ImGui::Text("f(x) = "); ImGui::SameLine();
		ImGui::Text((std::to_string(y)).c_str());
	}
}
void inquire_P()
{
	static char char_x[MAX_INPUT_NUMBER * sizeof(double)] = "\0";
	extern int nofP;
	std::vector<double> a =Create_Polynomial_Least_Squres(nofP,num_X, num_Y);
	struct TextFilters {
		static int FilterImGuiLetters(ImGuiInputTextCallbackData* data) {
			if (data->EventChar < 256 && strchr("1234567890. \n", (char)data->EventChar))
				return 0;
			return 1;
		}
	};
	ImGui::SetNextItemWidth(INPUT_WIDTH_XANDFX);
	ImGui::InputText("x", char_x, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
	ImGui::SameLine();
	if (ImGui::Button("OK") && strcmp(char_x, "\0"))
	{

		show_inquire_P = true;
	}
	if (show_inquire_P)
	{
		double num_x = std::stod(char_x);
		double y = Compute_Polynomial_Least_Squres(a,  num_x);

		ImGui::Text("\n\n");
		ImGui::Text("f(x) = "); ImGui::SameLine();
		ImGui::Text((std::to_string(y)).c_str());
	}
}

void File(void)
{
	std::ifstream f;
	std::string s;
	int i = 0;
	f.open("data.csv", std::ios::in);
	if (f.is_open())
	{
		num_X.clear(); num_Y.clear();
		while (std::getline(f, s))
		{
			num_X.push_back(stod(s));
			while (!s.empty() && s[0] != ',')
				s.erase(0, 1);

			s.erase(0, 1);
			num_Y.push_back(stod(s));
			i++;
		}
	}
}