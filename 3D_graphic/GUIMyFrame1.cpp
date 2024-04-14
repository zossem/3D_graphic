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
    double window_x = WxPanel->GetSize().x, window_y = WxPanel->GetSize().y;
    double n = 0.1, f= 100.0, r = 1.0, l = -1.0, t = 1.0, b = -1.0;
    //double n = 2.0, f = 0.1, r = window_x, l = 0.0, t = window_y, b =0.0;
    Vector4 set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4;
    Matrix4 MVP_matrix;
    /*set_matrix_v_1.Set(2.0 * n / (r - l), 0.0, 0.0);
    set_matrix_v_2.Set(0.0, 2.0 * n / (t - b), 0.0);
    set_matrix_v_3.Set((r+l)/(r-l), (t+b)/ (t - b), -1.0*(f+n)/(f-n));
    set_matrix_v_4.Set(0.0, 0.0, -2.0*f*n/(f-n));*/
    double alfa = (M_PI) / 2.0, a_r = window_x / window_y;
    set_matrix_v_1.Set(1/(a_r*tan(alfa/2.0)), 0.0, 0.0);
    set_matrix_v_2.Set(0.0, 1/tan(alfa/2.0), 0.0);
    set_matrix_v_3.Set(0.0, 0.0, (f + n) / (f - n));
    set_matrix_v_4.Set(0.0, 0.0, 2.0 * f * n / (f - n));
    SetMatrix(MVP_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);
    MVP_matrix.data[3][2] = -1.0;
    MVP_matrix.data[3][3] = 0.0;

    double Sx = (WxSB_ScaleX->GetValue() / 100.0);// *(window_x / 2.0);
    double Sy = (WxSB_ScaleY->GetValue() / 100.0);// *(window_y / 2.0);
    double Sz = (WxSB_ScaleZ->GetValue() / 100.0) ;
    Matrix4 Scale_matrix;
    set_matrix_v_1.Set(Sx, 0.0, 0.0);
    set_matrix_v_2.Set(0.0, Sy, 0.0);
    set_matrix_v_3.Set(0.0, 0.0, Sz);
    set_matrix_v_4.Set(0.0, 0.0, 0.0);
    SetMatrix(Scale_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);

    double Rx = (360.0 - WxSB_RotateX->GetValue())/180.0*M_PI;
    double Ry = (WxSB_RotateY->GetValue())/ 180.0 * M_PI;
    double Rz = (WxSB_RotateZ->GetValue())/ 180.0 * M_PI;
    Matrix4 Rot_X_matrix;
    set_matrix_v_1.Set(1.0, 0.0, 0.0);
    set_matrix_v_2.Set(0.0, cos(Rx), sin(Rx));
    set_matrix_v_3.Set(0.0, -sin(Rx), cos(Rx));
    set_matrix_v_4.Set(0.0, 0.0, 0.0);
    SetMatrix(Rot_X_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);
    Matrix4 Rot_Y_matrix;
    set_matrix_v_1.Set(cos(Ry), 0.0, -sin(Ry));
    set_matrix_v_2.Set(0.0, 1.0, 0.0);
    set_matrix_v_3.Set(sin(Ry), 0.0, cos(Ry));
    set_matrix_v_4.Set(0.0, 0.0, 0.0);
    SetMatrix(Rot_Y_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);
    Matrix4 Rot_Z_matrix;
    set_matrix_v_1.Set(cos(Rz), sin(Rz),0.0);
    set_matrix_v_2.Set(-sin(Rz), cos(Rz), 0.0);
    set_matrix_v_3.Set(0.0, 0.0, 1.0);
    set_matrix_v_4.Set(0.0, 0.0, 0.0);
    SetMatrix(Rot_Z_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);

    double Tx=(WxSB_TranslationX->GetValue() - 100.0) / 50.0;
    double Ty= (WxSB_TranslationY->GetValue() - 100.0) / 50.0;
    double Tz=  (WxSB_TranslationZ->GetValue() - 100.0) / 50.0;
    Matrix4 Trans_matrix;
    set_matrix_v_1.Set(1.0, 0.0, 0.0);
    set_matrix_v_2.Set(0.0, 1.0, 0.0);
    set_matrix_v_3.Set(0.0, 0.0, 1.0);
    set_matrix_v_4.Set(Tx, Ty, Tz);
    SetMatrix(Trans_matrix, set_matrix_v_1, set_matrix_v_2, set_matrix_v_3, set_matrix_v_4);

    //DrawWithTransformation(MVP_matrix * Rot_Z_matrix * Rot_Y_matrix * Rot_X_matrix * Scale_matrix);
    DrawWithTransformation(Rot_Z_matrix * Rot_Y_matrix * Rot_X_matrix *  Scale_matrix, MVP_matrix , Trans_matrix);
// tu rysowac
}



void GUIMyFrame1::DrawWithTransformation(Matrix4 transformation, Matrix4 prspective, Matrix4 translation)
{
    double window_x = WxPanel->GetSize().x, window_y = WxPanel->GetSize().y;
    wxClientDC dc1(WxPanel);
    wxBufferedDC dc(&dc1);

    dc.SetBackground(wxBrush(RGB(255, 255, 255)));
    dc.Clear();

    Vector4 v_eye, v_center, v_up;
    v_eye.Set(0.0, 0.0, 0.0); v_center.Set(0.0, 0.0, 1.1); v_up.Set(0.0, 1.0, 0.0);
    Matrix4 look_matrix = LookAt(v_eye, v_center, v_up);

    Vector4 v_begin, v_end;
    wxColor color_line;
    for (int i = 0; i < data.size(); i++)
    {
        v_begin.Set( data[i].begin.x, data[i].begin.y, data[i].begin.z);
        v_end.Set(data[i].end.x, data[i].end.y, data[i].end.z);
        color_line = wxColor(data[i].color.R, data[i].color.G, data[i].color.B);
        dc.SetPen(color_line);

        v_begin = transformation * v_begin;
        v_end = transformation * v_end;

        double barier = 0.1;
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
            double length = fabs((barier- v_out.GetZ()/(v_in.GetZ()- v_out.GetZ())));
            v_out.Set(v_in.GetX()-v_out.GetX()*length + v_out.GetX(), v_in.GetY() - v_out.GetY() * length + v_out.GetY(), barier);

            v_begin = v_out;
            v_end = v_in;

            v_begin = prspective * v_begin;
            v_end = prspective * v_end;
            v_begin.Set(v_begin.GetX()/ v_begin.data[3], v_begin.GetY() / v_begin.data[3], v_begin.GetZ() / v_begin.data[3]);
            v_end.Set(v_end.GetX() / v_end.data[3], v_end.GetY() / v_end.data[3], v_end.GetZ() / v_end.data[3]);

            Matrix4 scale_matrix;
            scale_matrix.data[0][0] = window_x / 2.0;
            scale_matrix.data[1][1] = window_y / 2.0;
            scale_matrix.data[2][2] = 1.0;
            scale_matrix.data[3][3] = 1.0;

            v_begin = translation* look_matrix * scale_matrix* v_begin;
            v_end = translation * look_matrix * scale_matrix* v_end;
            dc.DrawLine(v_begin.GetX()  + window_x / 2.0, v_begin.GetY()  + window_y / 2.0, v_end.GetX()  + window_x / 2.0, v_end.GetY()  + window_y / 2.0);

        }
        else if (v_begin.GetZ() <= barier && v_end.GetZ() <= barier)
        {

        }
        else
        {
            v_begin = prspective * v_begin;
            v_end = prspective * v_end;
            v_begin.Set(v_begin.GetX() / v_begin.data[3], v_begin.GetY() / v_begin.data[3], v_begin.GetZ() / v_begin.data[3]);
            v_end.Set(v_end.GetX() / v_end.data[3], v_end.GetY() / v_end.data[3], v_end.GetZ() / v_end.data[3]);

            Matrix4 scale_matrix;
            scale_matrix.data[0][0] = window_x / 2.0;
            scale_matrix.data[1][1] = window_y / 2.0;
            scale_matrix.data[2][2] = 1.0;
            scale_matrix.data[3][3] = 1.0;

            v_begin = translation * look_matrix * scale_matrix * v_begin;
            v_end = translation * look_matrix * scale_matrix * v_end;

            dc.DrawLine(v_begin.GetX() + window_x / 2.0, v_begin.GetY() + window_y / 2.0, v_end.GetX() + window_x / 2.0, v_end.GetY() + window_y / 2.0);
        }

        
        //dc.DrawLine(v_begin.GetX()  + window_x / 2.0, v_begin.GetY()  + window_y / 2.0, v_end.GetX()  + window_x / 2.0, v_end.GetY() + window_y / 2.0);
        
        
    }
}

Matrix4 GUIMyFrame1::LookAt(Vector4 eye, Vector4 center, Vector4 up)
{
    Matrix4 matrix;
    Vector4 v_X, v_Y, v_Z, v_W;
    v_Z = eye - center;
    double norm = norm_vect(v_Z);
    v_Z.Set(v_Z.GetX() / norm, v_Z.GetY() / norm, v_Z.GetZ() / norm);
    v_Y = up;
    v_X = vector_product(v_Y, v_Z);
    v_Y = vector_product(v_Z, v_X);
    norm= norm_vect(v_X);
    v_X.Set(v_X.GetX() / norm, v_X.GetY() / norm, v_X.GetZ() / norm);
    norm = norm_vect(v_Y);
    v_Y.Set(v_Y.GetX() / norm, v_Y.GetY() / norm, v_Y.GetZ() / norm);
    v_W.Set(0.0, 0.0, 0.0);
    SetMatrix(matrix, v_X, v_Y, v_Z, v_W);
    matrix.data[3][0] = -1.0 * scalar_product(v_X , eye);
    matrix.data[3][1] = -1.0 * scalar_product(v_Y , eye);
    matrix.data[3][2] = -1.0 * scalar_product(v_Z , eye);
    return matrix;
}

double GUIMyFrame1::norm_vect(Vector4& a)
{
    return sqrt(a.GetX() * a.GetX() + a.GetY() * a.GetY() + a.GetZ() * a.GetZ());
}

Vector4 GUIMyFrame1::vector_product(Vector4& a, Vector4& b)
{
    Vector4 product;
    product.Set(a.GetY()*b.GetZ()- a.GetZ() * b.GetY(), a.GetZ() * b.GetX() - a.GetX() * b.GetZ(), a.GetX() * b.GetY() - a.GetY() * b.GetX());
    return product;
}

double GUIMyFrame1::scalar_product(Vector4& a, Vector4& b)
{
    double product;
    product=(a.GetX() * b.GetX() + a.GetY() * b.GetY()+ a.GetZ() * b.GetZ());
    return product;
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