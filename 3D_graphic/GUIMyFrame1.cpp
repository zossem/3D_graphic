#include "GUIMyFrame1.h"
#include <vector>
#include <fstream>
#include <cmath>

# define M_PI 3.14159265358979323846

struct Point {
 float x, y, z;
 Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Color {
 int R, G, B;
 Color(int _R, int _G, int _B) : R(_R), G(_G), B(_B) {}
};

struct Segment {
 Point begin, end;
 Color color;
 Segment(Point _begin, Point _end, Color _color) : begin(_begin), end(_end), color(_color) {}
};

std::vector<Segment> data;

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
 m_button_load_geometry->SetLabel(_("Wczytaj Geometri\u0119"));
 m_staticText25->SetLabel(_("Obr\u00F3t X:"));
 m_staticText27->SetLabel(_("Obr\u00F3t Y:"));
 m_staticText29->SetLabel(_("Obr\u00F3t Z:"));

 WxSB_TranslationX->SetRange(0, 200); WxSB_TranslationX->SetValue(100);
 WxSB_TranslationY->SetRange(0, 200); WxSB_TranslationY->SetValue(100);
 WxSB_TranslationZ->SetRange(0, 200); WxSB_TranslationZ->SetValue(100);

 WxSB_RotateX->SetRange(0, 360); WxSB_RotateX->SetValue(0);
 WxSB_RotateY->SetRange(0, 360); WxSB_RotateY->SetValue(0);
 WxSB_RotateZ->SetRange(0, 360); WxSB_RotateZ->SetValue(0);

 WxSB_ScaleX->SetRange(1, 200); WxSB_ScaleX->SetValue(100);
 WxSB_ScaleY->SetRange(1, 200); WxSB_ScaleY->SetValue(100);
 WxSB_ScaleZ->SetRange(1, 200); WxSB_ScaleZ->SetValue(100);
}

void GUIMyFrame1::WxPanel_Repaint( wxUpdateUIEvent& event )
{
 Repaint();
}

void GUIMyFrame1::m_button_load_geometry_click( wxCommandEvent& event )
{
 wxFileDialog WxOpenFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("Geometry file (*.geo)|*.geo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

 if (WxOpenFileDialog.ShowModal() == wxID_OK)
 {
  double x1, y1, z1, x2, y2, z2;
  int r, g, b;

  std::ifstream in(WxOpenFileDialog.GetPath().ToAscii());
  if (in.is_open())
  {
   data.clear();
   while (!in.eof())
   {
    in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> r >> g >> b;
    data.push_back(Segment(Point(x1, y1, z1), Point(x2, y2, z2), Color(r, g, b)));
   }
   in.close();
  }
 }
}

void GUIMyFrame1::Scrolls_Updated( wxScrollEvent& event )
{
WxST_TranslationX->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationX->GetValue() - 100) / 50.0));
WxST_TranslationY->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationY->GetValue() - 100) / 50.0));
WxST_TranslationZ->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationZ->GetValue() - 100) / 50.0));

WxST_RotateX->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateX->GetValue()));
WxST_RotateY->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateY->GetValue()));
WxST_RotateZ->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateZ->GetValue()));

WxST_ScaleX->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleX->GetValue() / 100.0));
WxST_ScaleY->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleY->GetValue() / 100.0));
WxST_ScaleZ->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleZ->GetValue() / 100.0));


Repaint();
}


void GUIMyFrame1::Repaint()
{
    double window_x = WxPanel->GetSize().x;
    double window_y = WxPanel->GetSize().y;
    Vector4 set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4;

    Matrix4 MVP_matrix;
    double alfa = (M_PI) / 2.0, a_r = window_x / window_y, n = 0.1, f = 100.0;
    set_matrix_v_1.Set(1/(a_r*tan(alfa/2.0)), 0.0, 0.0);
    set_matrix_v_2.Set(0.0, 1/tan(alfa/2.0), 0.0);
    set_matrix_v_3.Set(0.0, 0.0, -1.0*(f + n) / (f - n));
    set_matrix_v_4.Set(0.0, 0.0, -2.0 * f * n / (f - n));
    SetMatrix(MVP_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);
    MVP_matrix.data[3][2] = -1.0;
    MVP_matrix.data[3][3] = 0.0;

    double Sx = (WxSB_ScaleX->GetValue() / 100.0);
    double Sy = (WxSB_ScaleY->GetValue() / 100.0);
    double Sz = (WxSB_ScaleZ->GetValue() / 100.0);
    Matrix4 Scale_matrix;
    set_matrix_v_1.Set(Sx, 0.0, 0.0);
    set_matrix_v_2.Set(0.0, Sy, 0.0);
    set_matrix_v_3.Set(0.0, 0.0, Sz);
    set_matrix_v_4.Set(0.0, 0.0, 0.0);
    SetMatrix(Scale_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);

    double Rx = (360.0 - WxSB_RotateX->GetValue())/180.0*M_PI;
    Matrix4 Rot_X_matrix;
    set_matrix_v_1.Set(1.0, 0.0, 0.0);
    set_matrix_v_2.Set(0.0, cos(Rx), sin(Rx));
    set_matrix_v_3.Set(0.0, -sin(Rx), cos(Rx));
    set_matrix_v_4.Set(0.0, 0.0, 0.0);
    SetMatrix(Rot_X_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);

    double Ry = (WxSB_RotateY->GetValue()) / 180.0 * M_PI;
    Matrix4 Rot_Y_matrix;
    set_matrix_v_1.Set(cos(Ry), 0.0, -sin(Ry));
    set_matrix_v_2.Set(0.0, 1.0, 0.0);
    set_matrix_v_3.Set(sin(Ry), 0.0, cos(Ry));
    set_matrix_v_4.Set(0.0, 0.0, 0.0);
    SetMatrix(Rot_Y_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);

    double Rz = (WxSB_RotateZ->GetValue()) / 180.0 * M_PI;
    Matrix4 Rot_Z_matrix;
    set_matrix_v_1.Set(cos(Rz), sin(Rz),0.0);
    set_matrix_v_2.Set(-sin(Rz), cos(Rz), 0.0);
    set_matrix_v_3.Set(0.0, 0.0, 1.0);
    set_matrix_v_4.Set(0.0, 0.0, 0.0);
    SetMatrix(Rot_Z_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);


    double Tx = -1.0 * ((WxSB_TranslationX->GetValue() - 100.0) / 50.0);
    double Ty = ((WxSB_TranslationY->GetValue() - 100.0) / 50.0);
    double Tz = 1.0 + ((WxSB_TranslationZ->GetValue() - 100.0) / 50.0);


    Matrix4 scale_matrix_to_window;
    scale_matrix_to_window.data[0][0] = window_x/2.0;
    scale_matrix_to_window.data[1][1] = window_y/2.0;
    scale_matrix_to_window.data[2][2] = 1.0;
    scale_matrix_to_window.data[3][3] = 1.0;


    wxClientDC dc1(WxPanel);
    wxBufferedDC dc(&dc1);

    dc.SetBackground(wxBrush(RGB(255, 255, 255)));
    dc.Clear();


    Vector4 v_begin, v_end;
    wxColor color_line;
    for (int i = 0; i < data.size(); i++)
    {
        v_begin.Set(data[i].begin.x, data[i].begin.y, data[i].begin.z);
        v_end.Set(data[i].end.x, data[i].end.y, data[i].end.z);
        color_line = wxColor(data[i].color.R, data[i].color.G, data[i].color.B);
        dc.SetPen(color_line);


        v_begin = Rot_Z_matrix * Rot_Y_matrix * Rot_X_matrix * Scale_matrix * v_begin;
        v_end = Rot_Z_matrix * Rot_Y_matrix * Rot_X_matrix * Scale_matrix * v_end;

        v_begin.Set(v_begin.GetX() + Tx, v_begin.GetY() + Ty, v_begin.GetZ() + Tz);
        v_end.Set(v_end.GetX() + Tx, v_end.GetY() + Ty, v_end.GetZ() + Tz);


        double barier = 0.1;
        if (v_begin.GetZ() <= barier && v_end.GetZ() <= barier)
        {
        }
        else
        {
            if ((v_begin.GetZ() > barier && v_end.GetZ() <= barier) || (v_end.GetZ() > barier && v_begin.GetZ() <= barier))
            {
                Vector4 v_in, v_out;
                if (v_end.GetZ() <= barier)
                {
                    v_in = v_begin;
                    v_out = v_end;
                }
                else
                {
                    v_in = v_end;
                    v_out = v_begin;
                }
                double length = fabs((barier - v_out.GetZ() / (v_in.GetZ() - v_out.GetZ())));
                v_out.Set(v_in.GetX() - v_out.GetX() * length + v_out.GetX(), v_in.GetY() - v_out.GetY() * length + v_out.GetY(), barier);

                v_begin = v_out;
                v_end = v_in;
            }           


            v_begin = MVP_matrix * v_begin;
            v_end = MVP_matrix * v_end;

            v_begin.Set(v_begin.GetX() / v_begin.data[3], v_begin.GetY() / v_begin.data[3], v_begin.GetZ() / v_begin.data[3]);
            v_end.Set(v_end.GetX() / v_end.data[3], v_end.GetY() / v_end.data[3], v_end.GetZ() / v_end.data[3]);


            v_begin = scale_matrix_to_window * v_begin;
            v_end = scale_matrix_to_window * v_end;
            dc.DrawLine(v_begin.GetX() + window_x / 2.0, v_begin.GetY() + window_y / 2.0,
                        v_end.GetX() + window_x / 2.0, v_end.GetY() + window_y / 2.0);
        }        
    }
}

void GUIMyFrame1::SetMatrix(Matrix4& matrix, Vector4& v_1, Vector4& v_2, Vector4& v_3, Vector4& v_4)//v_1 to v_4 are vertical vectors, this enable to easily set the matrix
{
    matrix.data[0][0] = v_1.GetX();
    matrix.data[1][0] = v_1.GetY();
    matrix.data[2][0] = v_1.GetZ();
    matrix.data[0][1] = v_2.GetX();
    matrix.data[1][1] = v_2.GetY();
    matrix.data[2][1] = v_2.GetZ();
    matrix.data[0][2] = v_3.GetX();
    matrix.data[1][2] = v_3.GetY();
    matrix.data[2][2] = v_3.GetZ();
    matrix.data[0][3] = v_4.GetX();
    matrix.data[1][3] = v_4.GetY();
    matrix.data[2][3] = v_4.GetZ();
    matrix.data[3][3] = 1.0;
}