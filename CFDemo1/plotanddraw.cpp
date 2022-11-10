#include "plotanddraw.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "algorithm.h"
#include "implot.h"
#include "implot_internal.h"
#include "DataInput.h"
#include <vector>

#define SAMPLING_NUMBERS 500

extern std::vector<double> num_X, num_Y;
static double x_plot[SAMPLING_NUMBERS], y_plot[SAMPLING_NUMBERS];
static bool plotflag_L = 0;
static bool plotflag_D = 0;
static bool plotflag_C = 0;
static bool plotflag_P = 0;
int nofL = 1;
int nofD = 1;
int nofP = 1;
void drawMenu(void)
{
	ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );

	if (ImGui::Button("Open")) {
		File();
	}
	if (ImGui::Button("Save")) {

	}
	if (ImGui::Button("Export")) {

	}
	ImGui::End();
}

void Plotting(void)
{
	double max_Y = FindMax(num_Y);
	double max_X = FindMax(num_X);
	double min_Y = FindMin(num_Y);
	double min_X = FindMin(num_X);
	ImGui::Begin("Plotting", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (plotflag_L)
	{
		for (int i = 0; i < SAMPLING_NUMBERS; i++)
		{
			x_plot[i] = min_X + i * 1.0f * (max_X - min_X) / (1.0f * SAMPLING_NUMBERS);
			y_plot[i] = L_interpolation(nofL, num_X, num_Y, x_plot[i]);
		}
		
		if (ImPlot::BeginPlot("Lagrange Interpolation"))
		{
			ImPlot::SetupAxes("x", "y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
			ImPlot::PlotLine(" ", x_plot, y_plot, SAMPLING_NUMBERS);
			ImPlot::EndPlot();
		}
	}
	
	if (plotflag_D)
	{
		static double x_plot[SAMPLING_NUMBERS], y_plot[SAMPLING_NUMBERS];
		for (int i = 0; i < SAMPLING_NUMBERS; i++)
		{
			x_plot[i] = min_X + i * 1.0f * (max_X - min_X) / (1.0f * SAMPLING_NUMBERS);
			y_plot[i] = Divided_Difference(nofD, num_X, num_Y, x_plot[i]);
		}

		if (ImPlot::BeginPlot("Divided Difference Interpolation"))
		{
			ImPlot::SetupAxes("x", "y",  ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
			ImPlot::PlotLine(" ", x_plot, y_plot, SAMPLING_NUMBERS);
			ImPlot::EndPlot();
		}
	}
	if (plotflag_C)
	{
		std::vector<std::vector<double>> a = Create_Cubic_Spline_Interpolation(num_X, num_Y);
		for (int i = 0; i < SAMPLING_NUMBERS; i++)
		{
			x_plot[i] = min_X + i * 1.0f * (max_X - min_X) / (1.0f * SAMPLING_NUMBERS);
			y_plot[i] = Compute_Cubic_Spline( a, num_X, x_plot[i]);
		}

		if (ImPlot::BeginPlot("Cubic Spline Interpolation"))
		{
			ImPlot::SetupAxes("x", "y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
			ImPlot::PlotLine(" ", x_plot, y_plot, SAMPLING_NUMBERS);
			ImPlot::EndPlot();
		}
	}
	if (plotflag_P)
	{
		std::vector<double> a = Create_Polynomial_Least_Squres(nofP, num_X, num_Y);
		for (int i = 0; i < SAMPLING_NUMBERS; i++)
		{
			x_plot[i] = min_X + i * 1.0f * (max_X - min_X) / (1.0f * SAMPLING_NUMBERS);
			y_plot[i] = Compute_Polynomial_Least_Squres(a, x_plot[i]);
		}

		if (ImPlot::BeginPlot("Ploynomial Least Squares Approximation"))
		{
			ImPlot::SetupAxes("x", "y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
			ImPlot::PlotLine(" ", x_plot, y_plot, SAMPLING_NUMBERS);
			ImPlot::EndPlot();
		}
	}
	ImGui::End();
}

void FittingOptions(void)
{
	
	ImGui::Begin("Fitting Options", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	const char* item_names[] =
	{
		"Lagrange Interpolating Polynomial", "Divided Difference Interpolating Poltnomial","Cubic Spline Interpolation", "Least Squre Polynomials"
	};
	static int item_type = 0;
	ImGui::Combo(" ", &item_type, item_names, IM_ARRAYSIZE(item_names), IM_ARRAYSIZE(item_names));

	if (item_type == 0)
	{
		ImGui::Text("\n\n");
		ImGui::Separator();
		ImGui::Text("\n\nDegree of the Lagrange interpolating polynomail:\n      ");
		ImGui::DragInt("th", &nofL, 0.05f, 1, num_X.size()-1, "%d", ImGuiSliderFlags_Logarithmic);
		ImGui::Text("\n\n\n");
		if (ImGui::Button("Create!") && !num_X.empty())
		{
			plotflag_L = 1;
			plotflag_D = 0;
			plotflag_C = 0;
			plotflag_P = 0;
		}
		ImGui::Text("\n\n");
		ImGui::Separator();
		ImGui::Text("\n\nInquire: (Please press Create! button first)\n      ");
		inquire_L();
	}
	if (item_type == 1) {
		ImGui::Text("\n\n");
		ImGui::Separator();
		ImGui::Text("\n\nDegree of the Divided difference interpolating polynomail:\n      ");
		ImGui::DragInt("th", &nofD, 0.05f, 1, num_X.size()-1, "%d", ImGuiSliderFlags_Logarithmic);
		ImGui::Text("\n\n\n");
		if (ImGui::Button("Create!") && !num_X.empty())
		{
			plotflag_L = 0;
			plotflag_D = 1;
			plotflag_C = 0;
			plotflag_P = 0;
		}
		ImGui::Text("\n\n");
		ImGui::Separator();
		ImGui::Text("\n\nInquire: (Please press Create! button first)\n      ");
		inquire_D();

	}                                   
	if (item_type == 2)
	{
		ImGui::Text("\n\n");
		ImGui::Separator();
		ImGui::Text("\n\n");
		if (ImGui::Button("Create!") && !num_X.empty())
		{
			plotflag_L = 0;
			plotflag_D = 0;
			plotflag_C = 1;
			plotflag_P = 0;
		}
		ImGui::Text("\n\n");
		ImGui::Separator();
		ImGui::Text("\n\nInquire: (Please press Create! button first)\n      ");
		inquire_C();
	}
	if (item_type == 3)
	{
		ImGui::Text("\n\n");
		ImGui::Separator();
		ImGui::Text("\n\nDegree of the polynomail least squares: \n     ");
		ImGui::DragInt("th", &nofP, 0.05f, 1, num_X.size()-2, "%d", ImGuiSliderFlags_Logarithmic);
		ImGui::Text("\n\n\n");
		if (ImGui::Button("Create!")&& !num_X.empty())
		{
			plotflag_L = 0;
			plotflag_D = 0;
			plotflag_C = 0;
			plotflag_P = 1;
		}
		ImGui::Text("\n\n");
		ImGui::Separator();
		ImGui::Text("\n\nInquire: (Please press Create! button first)\n      ");
		inquire_P();
	}

	ImGui::End();
}