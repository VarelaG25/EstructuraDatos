#include <Windows.h>
#include "resource.h"
#include "framework.h"
#include "Nuevo.h"
#include <Commctrl.h>
#include <set>
#include <cmath>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include "ArbolBinario.h"
#include "Vuelos.h"
#include "Pasajeros.h"
#include "Boletos.h"
#pragma comment(lib, "Comctl32.lib")

#define MI_TIMER_INVENTADO 117
#define MAX_LOADSTRING 100



HINSTANCE Ginst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal
ArbolBinario arbol;
UsuarioAdmin adminPredeterminado;
UsuarioActual usuarioActual;
ListaDoblePasajero listaPasajero;
ListaDobleVuelo ListaVuelos;
ListaDobleAsiento ListaAsientos;
ListaDobleBoleto ListaBoletos;
// Variables globales:
set<int> numerosUsados;
int Random;
char rutaGeneral[200] = "C:/Users/marbr/source/repos/Nuevo (1)/Nuevo/pasajeros.bin";
char rutaGeneral2[200] = "C:/Users/marbr/source/repos/Nuevo (1)/Nuevo/vuelos.bin";
char rutaGeneral3[200] = "C:/Users/marbr/source/repos/Nuevo (1)/Nuevo/usuarios.bin";
char rutaGeneral4[200] = "C:/Users/marbr/source/repos/Nuevo (1)/Nuevo/asientosporavion.bin";
char rutaGeneral5[200] = "C:/Users/marbr/source/repos/Nuevo (1)/Nuevo/ultimoUser.bin";
char rutaGeneral6[200] = "C:/Users/marbr/source/repos/Nuevo (1)/Nuevo/boletos.bin";
bool guardarUsuario = false;

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
//Callbacks
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK InicioSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Usuarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Principal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Vuelos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Boletos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Pasajeros(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//Funciones
bool menu(int opcion, HWND ventana);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {

	HWND hVentana1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_IniciarSesion), NULL, InicioSesion);
	Ginst = hInst;
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	arbol.cargar(rutaGeneral3);
	arbol.escribirEnArchivoTexto("users.txt");
	listaPasajero.cargar(rutaGeneral);
	ListaVuelos.cargar(rutaGeneral2);
	ListaAsientos.cargar(rutaGeneral4);
	ListaBoletos.cargar(rutaGeneral6);
	srand(static_cast<unsigned int>(time(nullptr))); // Inicializa el generador de números aleatorios

	ShowWindow(hVentana1, cShow);

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

INT_PTR CALLBACK InicioSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {
		UsuarioActual usuarioGuardado;
		if (cargarUsuarioActual(usuarioGuardado, rutaGeneral5)) {
			SetWindowTextA(GetDlgItem(hwnd, IDC_EDIT2), usuarioGuardado.Username);
			SetWindowTextA(GetDlgItem(hwnd, IDC_EDIT3), usuarioGuardado.Contrasenia);
			CheckDlgButton(hwnd, IDC_CHECK1, BST_CHECKED);
			SetWindowTextA(GetDlgItem(hwnd, US_ActualUser), usuarioGuardado.Username);
			cargarImagenUsuario(hwnd, usuarioGuardado.RutaImagen);
			guardarUsuario = true;
		}
	}break;

	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		switch (wmId) {
		case IDC_CHECK1: {
			if (wmEvent == BN_CLICKED) {
				guardarUsuario = IsDlgButtonChecked(hwnd, IDC_CHECK1) == BST_CHECKED;
			}
		} break;
		case BTN_Iniciar: {
			char usuario[MAX_PATH];
			char contrasenia[MAX_PATH];
			GetWindowTextA(GetDlgItem(hwnd, IDC_EDIT2), usuario, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, IDC_EDIT3), contrasenia, MAX_PATH);

			if (strcmp(usuario, adminPredeterminado.usuarioAdmin) == 0 && strcmp(contrasenia, adminPredeterminado.ContraseniaAdmin) == 0) {
				MessageBoxA(NULL, "Entrando como administrador.", "Inicio de sesion", MB_OK | MB_ICONINFORMATION);
				strcpy_s(usuarioActual.Username, adminPredeterminado.usuarioAdmin);
				usuarioActual.Edad = adminPredeterminado.EdadAdmin;
				strcpy_s(usuarioActual.Correo, adminPredeterminado.CorreoAdmin);
				strcpy_s(usuarioActual.Contrasenia, adminPredeterminado.ContraseniaAdmin);
				strcpy_s(usuarioActual.Nombre_U, adminPredeterminado.Nombre_UAdmin);
				strcpy_s(usuarioActual.Apellido_P, adminPredeterminado.Apellido_PAdmin);
				strcpy_s(usuarioActual.Apellido_M, adminPredeterminado.Apellido_MAdmin);
				strcpy_s(usuarioActual.fechaNacimiento, adminPredeterminado.FechaNacimientoAdmin);
				strcpy_s(usuarioActual.Genero, adminPredeterminado.GeneroAdmin);
				strcpy_s(usuarioActual.RutaImagen, adminPredeterminado.RutaFotoAdmin);

				if (guardarUsuario) {
					guardarUsuarioActual(usuarioActual, rutaGeneral5);
				}

				HWND ventanaAdmin = CreateDialog(Ginst, MAKEINTRESOURCE(DLG_Principal), NULL, Principal);
				ShowWindow(ventanaAdmin, SW_SHOW);
				EndDialog(hwnd, 0);
			}
			else {
				NodoUsuario* usuarioIngresado = arbol.buscar(usuario);
				if (usuarioIngresado != nullptr && strcmp(contrasenia, usuarioIngresado->usuario.Contrasenia) == 0) {
					strcpy_s(usuarioActual.Username, usuarioIngresado->usuario.Username);
					usuarioActual.Edad = usuarioIngresado->usuario.Edad;
					strcpy_s(usuarioActual.Correo, usuarioIngresado->usuario.Correo);
					strcpy_s(usuarioActual.Contrasenia, usuarioIngresado->usuario.Contrasenia);
					strcpy_s(usuarioActual.Nombre_U, usuarioIngresado->usuario.Nombre);
					strcpy_s(usuarioActual.Apellido_P, usuarioIngresado->usuario.ApellidoP);
					strcpy_s(usuarioActual.Apellido_M, usuarioIngresado->usuario.ApellidoM);
					strcpy_s(usuarioActual.fechaNacimiento, usuarioIngresado->usuario.FechaNacimiento);
					strcpy_s(usuarioActual.Genero, usuarioIngresado->usuario.Genero);
					strcpy_s(usuarioActual.RutaImagen, usuarioIngresado->usuario.RutaImagen);

					if (guardarUsuario) {
						guardarUsuarioActual(usuarioActual, rutaGeneral5);
					}

					MessageBoxA(NULL, "Inicio de sesion exitoso.", "Bienvenido", MB_OK | MB_ICONINFORMATION);
					HWND ventanaUsuario = CreateDialog(Ginst, MAKEINTRESOURCE(DLG_Principal), NULL, Principal);
					ShowWindow(ventanaUsuario, SW_SHOW);
					EndDialog(hwnd, 0);
				}
				else {
					MessageBoxA(NULL, "Credenciales incorrectas.", "Error", MB_OK | MB_ICONERROR);
				}
			}
		} break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	} break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(117);
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK Principal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		// Cargar la nueva imagen del usuario actual
		HBITMAP nuevaImagen = (HBITMAP)LoadImageA(NULL, usuarioActual.RutaImagen, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		if (nuevaImagen != NULL) {
			// Establecer la nueva imagen en el control de imagen correspondiente
			SendMessage(GetDlgItem(hwnd, US_ActualImage), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)nuevaImagen);
		}

		// Obtener el identificador del control de edición US_ActualUser
		HWND editControl = GetDlgItem(hwnd, US_ActualUser);

		if (editControl != NULL) {
			// Establecer el texto del control con el nombre de usuario actual
			SetWindowTextA(editControl, usuarioActual.Username);
		}

	} break;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		if (menu(wmId, hwnd))
			return FALSE;
		switch (wmId) {
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	} break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(117);
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK Usuarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char username[MAX_PATH], edad[MAX_PATH], correo[MAX_PATH], contrasenia[MAX_PATH], nombre[MAX_PATH], aPaterno[MAX_PATH], aMaterno[MAX_PATH],
		fechaNacimiento[MAX_PATH], fechaRegistro[MAX_PATH], genero[MAX_PATH], rutaImagen[MAX_PATH], hora[MAX_PATH], fecha[MAX_PATH];
	switch (msg)
	{
	case WM_INITDIALOG: {

		// Cargar la nueva imagen del usuario actual
		HBITMAP nuevaImagen = (HBITMAP)LoadImageA(NULL, usuarioActual.RutaImagen, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		if (nuevaImagen != NULL) {
			// Establecer la nueva imagen en el control de imagen correspondiente
			SendMessage(GetDlgItem(hwnd, US_ActualImage), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)nuevaImagen);
		}

		// Obtener el identificador del control de edición US_ActualUser
		HWND editControl = GetDlgItem(hwnd, US_ActualUser);

		if (editControl != NULL) {
			// Establecer el texto del control con el nombre de usuario actual
			SetWindowTextA(editControl, usuarioActual.Username);
		}

		// Obtener el handle del ListBox donde se mostrarán los usuarios
		HWND userListBox = GetDlgItem(hwnd, EU_Lista);

		// Verificar si se pudo obtener el handle del ListBox
		if (userListBox != NULL) {
			// Limpiar la lista de usuarios en caso de que haya elementos previos
			SendMessage(userListBox, LB_RESETCONTENT, 0, 0);

			// Recorrer la lista de usuarios y agregar cada uno al ListBox
			for (NodoUsuario* usuario = arbol.getPrimerUsuario(); usuario != nullptr; usuario = arbol.getSiguienteUsuario(usuario)) {
				// Obtener el nombre de usuario del usuario actual
				const char* username = usuario->usuario.Username;

				// Agregar el nombre de usuario al ListBox
				int index = SendMessageA(userListBox, LB_ADDSTRING, 0, (LPARAM)username);
				// Establecer el puntero al nodo del usuario como el dato asociado al índice
				SendMessage(userListBox, LB_SETITEMDATA, index, (LPARAM)usuario);
			}
		}

		HMENU menu = LoadMenu(Ginst, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, menu);

		// Obtener el handle del ComboBox de género usando el identificador del recurso de diálogo
		HWND genero1 = GetDlgItem(hwnd, US_Genero);
		HWND genero2 = GetDlgItem(hwnd, MD_NuevoGenero);

		// Agregar elementos al ComboBox
		SendMessageA(genero2, CB_ADDSTRING, 0, (LPARAM)"Masculino");
		SendMessageA(genero2, CB_ADDSTRING, 0, (LPARAM)"Femenino");

		// Agregar elementos al ComboBox
		SendMessageA(genero1, CB_ADDSTRING, 0, (LPARAM)"Masculino");
		SendMessageA(genero1, CB_ADDSTRING, 0, (LPARAM)"Femenino");

		if (strlen(usuarioActual.Username) > 0) {
			// Convertir cadenas de caracteres normales a cadenas anchas
			wchar_t username[MAX_PATH], correo[MAX_PATH], nombre[MAX_PATH], aPaterno[MAX_PATH], aMaterno[MAX_PATH];
			wchar_t fechaNacimiento[MAX_PATH], genero[MAX_PATH], rutaImagen[MAX_PATH], contrasenia[MAX_PATH];
			int edad = usuarioActual.Edad;

			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, username, usuarioActual.Username, _TRUNCATE);
			mbstowcs_s(&convertedChars, contrasenia, usuarioActual.Contrasenia, _TRUNCATE);
			mbstowcs_s(&convertedChars, correo, usuarioActual.Correo, _TRUNCATE);
			mbstowcs_s(&convertedChars, nombre, usuarioActual.Nombre_U, _TRUNCATE);
			mbstowcs_s(&convertedChars, aPaterno, usuarioActual.Apellido_P, _TRUNCATE);
			mbstowcs_s(&convertedChars, aMaterno, usuarioActual.Apellido_M, _TRUNCATE);
			mbstowcs_s(&convertedChars, fechaNacimiento, usuarioActual.fechaNacimiento, _TRUNCATE);
			mbstowcs_s(&convertedChars, genero, usuarioActual.Genero, _TRUNCATE);
			mbstowcs_s(&convertedChars, rutaImagen, usuarioActual.RutaImagen, _TRUNCATE);

			// Mostrar la información del usuario actual en los edit controls
			SetDlgItemTextW(hwnd, MD_Username, username);
			SetDlgItemTextW(hwnd, MD_Contrasenia, username);
			SetDlgItemTextW(hwnd, MD_Correos, correo);
			SetDlgItemTextW(hwnd, MD_Nombre, nombre);
			SetDlgItemTextW(hwnd, MD_ApellidoP, aPaterno);
			SetDlgItemTextW(hwnd, MD_ApellidoM, aMaterno);
			SetDlgItemTextW(hwnd, MD_FechaNacimiento, fechaNacimiento);
			SetDlgItemTextW(hwnd, MD_RutaImagen, rutaImagen);
			SetDlgItemInt(hwnd, MD_Edad, edad, FALSE);

			// Seleccionar el género correspondiente en el ComboBox
			HWND hComboBox = GetDlgItem(hwnd, MD_Genero);
			if (hComboBox != NULL) {
				int index = SendMessageW(hComboBox, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)genero);
				if (index != CB_ERR) {
					SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)index, 0);
				}
			}

			if (strlen(usuarioActual.RutaImagen) > 0) {
				HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, usuarioActual.RutaImagen, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				if (hBitmap != NULL) {
					SendMessage(GetDlgItem(hwnd, MD_Img), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
				}
			}

		}

	} break;

	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		if (menu(wmId, hwnd))
			return FALSE;
		switch (wmId)
		{
		case EU_Lista: {
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				HWND hListBox = GetDlgItem(hwnd, EU_Lista);
				int selIndex = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				if (selIndex != LB_ERR) {
					// Obtener el puntero al usuario correspondiente al índice seleccionado
					NodoUsuario* nodoSeleccionado = (NodoUsuario*)SendMessage(hListBox, LB_GETITEMDATA, selIndex, 0);
					if (nodoSeleccionado != nullptr) {
						// Convertir cadenas de caracteres normales a cadenas anchas
						wchar_t username[MAX_PATH], correo[MAX_PATH], nombre[MAX_PATH], aPaterno[MAX_PATH], aMaterno[MAX_PATH];
						wchar_t fechaNacimiento[MAX_PATH], genero[MAX_PATH], rutaImagen[MAX_PATH], contrasenia[MAX_PATH], fechaRegistro[MAX_PATH];

						size_t convertedChars = 0;
						mbstowcs_s(&convertedChars, username, nodoSeleccionado->usuario.Username, _TRUNCATE);
						mbstowcs_s(&convertedChars, correo, nodoSeleccionado->usuario.Correo, _TRUNCATE);
						mbstowcs_s(&convertedChars, nombre, nodoSeleccionado->usuario.Nombre, _TRUNCATE);
						mbstowcs_s(&convertedChars, aPaterno, nodoSeleccionado->usuario.ApellidoP, _TRUNCATE);
						mbstowcs_s(&convertedChars, aMaterno, nodoSeleccionado->usuario.ApellidoM, _TRUNCATE);
						mbstowcs_s(&convertedChars, fechaNacimiento, nodoSeleccionado->usuario.FechaNacimiento, _TRUNCATE);
						mbstowcs_s(&convertedChars, genero, nodoSeleccionado->usuario.Genero, _TRUNCATE);
						mbstowcs_s(&convertedChars, rutaImagen, nodoSeleccionado->usuario.RutaImagen, _TRUNCATE);
						mbstowcs_s(&convertedChars, contrasenia, nodoSeleccionado->usuario.Contrasenia, _TRUNCATE);
						mbstowcs_s(&convertedChars, fechaRegistro, nodoSeleccionado->usuario.FechaRegistro, _TRUNCATE);

						// Mostrar la información del usuario seleccionado en los edit controls
						SetDlgItemTextW(hwnd, EU_Username, username);
						SetDlgItemInt(hwnd, EU_Edad, nodoSeleccionado->usuario.Edad, FALSE);
						SetDlgItemTextW(hwnd, EU_Correos, correo);
						SetDlgItemTextW(hwnd, EU_Nombre, nombre);
						SetDlgItemTextW(hwnd, EU_ApellidoP, aPaterno);
						SetDlgItemTextW(hwnd, EU_ApellidoM, aMaterno);
						SetDlgItemTextW(hwnd, EU_FechaNacimiento, fechaNacimiento);
						SetDlgItemTextW(hwnd, EU_Genero, genero);
						SetDlgItemTextW(hwnd, EU_RutaImagen, rutaImagen);

						// Mostrar la información del usuario seleccionado en los edit controls
						SetDlgItemTextW(hwnd, MD_Username, username);
						SetDlgItemInt(hwnd, MD_Edad, nodoSeleccionado->usuario.Edad, FALSE);
						SetDlgItemTextW(hwnd, MD_Correos, correo);
						SetDlgItemTextW(hwnd, MD_Contrasenia, contrasenia);
						SetDlgItemTextW(hwnd, MD_Nombre, nombre);
						SetDlgItemTextW(hwnd, MD_ApellidoP, aPaterno);
						SetDlgItemTextW(hwnd, MD_ApellidoM, aMaterno);
						SetDlgItemTextW(hwnd, MD_Nacimiento, fechaNacimiento);
						SetDlgItemTextW(hwnd, MD_Genero, genero);
						SetDlgItemTextW(hwnd, MD_RutaImagen, rutaImagen);
						SetDlgItemTextW(hwnd, MD_FechaRegistro, fechaRegistro);

						// Cargar la imagen y mostrarla en el control de imagen
						HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, rutaImagen, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
						if (hBitmap != NULL) {
							// Obtener el handle del control de imagen
							HWND hImageControl = GetDlgItem(hwnd, EU_Img);
							HWND hImageControl2 = GetDlgItem(hwnd, MD_Img);
							// Establecer la imagen en el control de imagen
							SendMessage(hImageControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
							SendMessage(hImageControl2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
						}
						else {
							// Si falla la carga de la imagen, mostrar un mensaje de error
							MessageBox(hwnd, L"No se pudo cargar la imagen del usuario.", L"Error", MB_OK | MB_ICONERROR);
						}
					}
				}
			}
		}break;
		case EU_Eliminar: {
			HWND hListBox = GetDlgItem(hwnd, EU_Lista);
			int selIndex = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			if (selIndex != LB_ERR) {
				// Obtener el puntero al usuario correspondiente al índice seleccionado
				char selectedUsername[MAX_PATH];
				SendMessageA(hListBox, LB_GETTEXT, selIndex, (LPARAM)selectedUsername);

				// Eliminar el usuario del árbol
				if (arbol.eliminarUsuario(selectedUsername)) {
					// Eliminar el elemento del ListBox
					SendMessage(hListBox, LB_DELETESTRING, selIndex, 0);

					// Reescribir el archivo binario
					arbol.guardar(rutaGeneral3);

					// Limpiar los edit controls
					SetDlgItemText(hwnd, EU_Username, L"");
					SetDlgItemText(hwnd, EU_Edad, L"");
					SetDlgItemText(hwnd, EU_Correos, L"");
					SetDlgItemText(hwnd, EU_Nombre, L"");
					SetDlgItemText(hwnd, EU_ApellidoP, L"");
					SetDlgItemText(hwnd, EU_ApellidoM, L"");
					SetDlgItemText(hwnd, EU_FechaNacimiento, L"");
					SetDlgItemText(hwnd, EU_Genero, L"");
					SetDlgItemText(hwnd, EU_RutaImagen, L"");

					// Limpiar la imagen
					SendMessage(GetDlgItem(hwnd, EU_Img), STM_SETIMAGE, IMAGE_BITMAP, 0);

					MessageBoxA(hwnd, "Usuario eliminado con exito.", "Exito", MB_OK | MB_ICONINFORMATION);
				}
				else {
					MessageBoxA(hwnd, "No se pudo eliminar el usuario.", "Error", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBoxA(hwnd, "Seleccione un usuario para eliminar.", "Advertencia", MB_OK | MB_ICONWARNING);
			}
		} break;

		case US_Cargar: {
			// GUARDA LA FOTO EN LA ESTRUCTURA
			char ruta[300] = { 0 };
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAMEA);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = ruta;
			ofn.nMaxFile = sizeof(ruta);
			ofn.lpstrFilter = "Imagenes\0*.bmp\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileNameA(&ofn)) {
				SetDlgItemTextA(hwnd, US_RutaImagen, ruta);
				HBITMAP imagen = (HBITMAP)LoadImageA(Ginst, ruta, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
				if (imagen == NULL) {
					// Manejo de error, imprime el código de error
					DWORD error = GetLastError();
					printf("LoadImage failed with error %lu\n", error);
				}
				else {
					if (!SendMessage(GetDlgItem(hwnd, US_Img), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen)) {
						// Manejo de error, imprime el código de error
						DWORD error = GetLastError();
						printf("SendMessage failed with error %lu\n", error);
					}
				}
			}
		} break;
		case MD_Registrar: {
			// Obtener el nombre de usuario seleccionado en la lista
			int index = SendMessage(GetDlgItem(hwnd, EU_Lista), LB_GETCURSEL, 0, 0);
			if (index != LB_ERR) {
				char username[MAX_PATH];
				SendMessage(GetDlgItem(hwnd, EU_Lista), LB_GETTEXT, index, (LPARAM)username);

				// Obtener los nuevos valores ingresados por el usuario
				char nuevoUsername[MAX_PATH], nuevoCorreo[MAX_PATH], nuevoNombre[MAX_PATH], nuevoAPaterno[MAX_PATH], nuevoAMaterno[MAX_PATH];
				char nuevaFechaNacimiento[MAX_PATH], nuevoGenero[MAX_PATH], nuevaRutaImagen[MAX_PATH], nuevaContrasenia[MAX_PATH];
				int nuevaEdad;

				// Obtener los valores de los campos de la interfaz gráfica
				GetDlgItemTextA(hwnd, MD_Username, nuevoUsername, MAX_PATH);
				GetDlgItemTextA(hwnd, MD_Correos, nuevoCorreo, MAX_PATH);
				GetDlgItemTextA(hwnd, MD_Nombre, nuevoNombre, MAX_PATH);
				GetDlgItemTextA(hwnd, MD_ApellidoP, nuevoAPaterno, MAX_PATH);
				GetDlgItemTextA(hwnd, MD_ApellidoM, nuevoAMaterno, MAX_PATH);
				GetDlgItemTextA(hwnd, MD_NuevoGenero, nuevoGenero, MAX_PATH);
				GetDlgItemTextA(hwnd, MD_RutaImagen, nuevaRutaImagen, MAX_PATH);
				GetDlgItemTextA(hwnd, MD_Contrasenia, nuevaContrasenia, MAX_PATH);
				nuevaEdad = GetDlgItemInt(hwnd, MD_Edad, NULL, FALSE);

				// Obtener la fecha de nacimiento del DateTimePicker
				SYSTEMTIME sysTime;
				DateTime_GetSystemtime(GetDlgItem(hwnd, MD_FechaNacimientoNueva), &sysTime);
				char buffer[MAX_PATH];
				GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &sysTime, NULL, buffer, MAX_PATH);
				strcpy_s(nuevaFechaNacimiento, buffer);

				// Crear un nuevo usuario con los valores actualizados
				Usuario nuevoUsuario;
				strcpy_s(nuevoUsuario.Username, nuevoUsername);
				nuevoUsuario.Edad = nuevaEdad;
				strcpy_s(nuevoUsuario.Correo, nuevoCorreo);
				strcpy_s(nuevoUsuario.Nombre, nuevoNombre);
				strcpy_s(nuevoUsuario.ApellidoP, nuevoAPaterno);
				strcpy_s(nuevoUsuario.ApellidoM, nuevoAMaterno);
				strcpy_s(nuevoUsuario.FechaNacimiento, nuevaFechaNacimiento);
				strcpy_s(nuevoUsuario.Genero, nuevoGenero);
				strcpy_s(nuevoUsuario.RutaImagen, nuevaRutaImagen);
				strcpy_s(nuevoUsuario.Contrasenia, nuevaContrasenia);

				// Modificar el usuario seleccionado en el árbol
				NodoUsuario* nodoUsuario = arbol.buscar(username);
				if (nodoUsuario != nullptr) {
					// Actualizar los campos del nodo con los nuevos valores
					strcpy_s(nodoUsuario->usuario.Username, nuevoUsuario.Username);
					nodoUsuario->usuario.Edad = nuevoUsuario.Edad;
					strcpy_s(nodoUsuario->usuario.Correo, nuevoUsuario.Correo);
					strcpy_s(nodoUsuario->usuario.Contrasenia, nuevoUsuario.Contrasenia);
					strcpy_s(nodoUsuario->usuario.Nombre, nuevoUsuario.Nombre);
					strcpy_s(nodoUsuario->usuario.ApellidoP, nuevoUsuario.ApellidoP);
					strcpy_s(nodoUsuario->usuario.ApellidoM, nuevoUsuario.ApellidoM);
					strcpy_s(nodoUsuario->usuario.FechaNacimiento, nuevoUsuario.FechaNacimiento);
					strcpy_s(nodoUsuario->usuario.Genero, nuevoUsuario.Genero);
					strcpy_s(nodoUsuario->usuario.RutaImagen, nuevoUsuario.RutaImagen);

					// Luego, puedes reescribir el archivo binario si es necesario
					arbol.guardar(rutaGeneral3);

					// Mensaje de éxito
					MessageBox(hwnd, L"Usuario actualizado con exito.", L"Exito", MB_OK | MB_ICONINFORMATION);
				}
				else {
					// Mensaje de error si no se encuentra el usuario
					MessageBox(hwnd, L"No se encontró el usuario seleccionado en el árbol.", L"Error", MB_OK | MB_ICONERROR);
				}
			}
			else {
				// Mensaje de error si no se ha seleccionado ningún usuario en la lista
				MessageBox(hwnd, L"Por favor, seleccione un usuario de la lista.", L"Error", MB_OK | MB_ICONERROR);
			}
		} break;

		case MD_Cargar: {
			// GUARDA LA FOTO EN LA ESTRUCTURA
			char ruta[300] = { 0 };
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAMEA);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = ruta;
			ofn.nMaxFile = sizeof(ruta);
			ofn.lpstrFilter = "Imagenes\0*.bmp\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileNameA(&ofn)) {
				SetDlgItemTextA(hwnd, MD_RutaImagen, ruta);
				HBITMAP imagen = (HBITMAP)LoadImageA(Ginst, ruta, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
				if (imagen == NULL) {
					// Manejo de error, imprime el código de error
					DWORD error = GetLastError();
					printf("LoadImage failed with error %lu\n", error);
				}
				else {
					if (!SendMessage(GetDlgItem(hwnd, MD_Img), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen)) {
						// Manejo de error, imprime el código de error
						DWORD error = GetLastError();
						printf("SendMessage failed with error %lu\n", error);
					}
				}
			}
		} break;


		case US_Registrar: {
			GetWindowTextA(GetDlgItem(hwnd, US_Username), username, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, US_Edad), edad, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, US_Nombre), nombre, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, US_ApellidoP), aPaterno, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, US_ApellidoM), aMaterno, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, US_Correo), correo, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, US_Contrasenia), contrasenia, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, US_RutaImagen), rutaImagen, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, US_Genero), genero, MAX_PATH);

			// Obtener la fecha de registro en formato SYSTEMTIME
			SYSTEMTIME fechaRegistro;
			SendMessage(GetDlgItem(hwnd, US_FechaRegistro), DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaRegistro);
			GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &fechaRegistro, NULL, fecha, MAX_PATH);

			// Obtener la fecha de nacimiento en formato SYSTEMTIME
			SYSTEMTIME Nacimiento;
			SendMessage(GetDlgItem(hwnd, US_FechaNacimiento), DTM_GETSYSTEMTIME, 0, (LPARAM)&Nacimiento);

			// Convertir la fecha de nacimiento a una cadena de caracteres con formato
			GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &Nacimiento, NULL, fechaNacimiento, MAX_PATH);

			// Verificar si ya existe un usuario con el mismo nombre
			NodoUsuario* usuarioExistente = arbol.buscar(username);
			if (usuarioExistente != nullptr) {
				// El usuario ya existe, mostrar mensaje de error
				MessageBoxA(hwnd, "El nombre de usuario ya esta en uso. Por favor, elija otro.", "Error", MB_OK | MB_ICONERROR);
			}
			else {
				// Verificar que todos los campos estén completos
				if (strlen(username) == 0 || strlen(edad) == 0 || strlen(nombre) == 0 || strlen(aPaterno) == 0 ||
					strlen(aMaterno) == 0 || strlen(correo) == 0 || strlen(contrasenia) == 0 || strlen(rutaImagen) == 0 ||
					strlen(genero) == 0 || strlen(fecha) == 0 || strlen(hora) == 0 || strlen(fechaNacimiento) == 0) {
					// Mostrar un mensaje de advertencia si algún campo está vacío
					MessageBoxA(hwnd, "Todos los campos deben completarse.", "Advertencia", MB_OK | MB_ICONWARNING);
				}
				else {
					// Crear una nueva estructura de Usuario con los datos recolectados
					Usuario nuevoUsuario;
					strcpy_s(nuevoUsuario.Username, username);
					nuevoUsuario.Edad = atoi(edad);
					strcpy_s(nuevoUsuario.Nombre, nombre);
					strcpy_s(nuevoUsuario.ApellidoP, aPaterno);
					strcpy_s(nuevoUsuario.ApellidoM, aMaterno);
					strcpy_s(nuevoUsuario.Correo, correo);
					strcpy_s(nuevoUsuario.Contrasenia, contrasenia);
					strcpy_s(nuevoUsuario.RutaImagen, rutaImagen);
					strcpy_s(nuevoUsuario.Genero, genero);
					strcpy_s(nuevoUsuario.FechaNacimiento, fechaNacimiento);
					strcpy_s(nuevoUsuario.FechaRegistro, fecha);

					// Insertar el nuevo usuario en el árbol binario de búsqueda
					arbol.insertar(nuevoUsuario);

					// Guardar el árbol actualizado en el archivo binario
					arbol.guardar(rutaGeneral3);

					// Mostrar un mensaje de éxito
					MessageBoxA(hwnd, "Usuario registrado con exito.", "Exito", MB_OK | MB_ICONINFORMATION);
				}
			}
		} break;

		default:
			return FALSE;
		}
	} break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(117);
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK Vuelos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char origen[MAX_PATH], destino[MAX_PATH], fechaorigen[MAX_PATH], horaorigen[MAX_PATH], fechadestino[MAX_PATH], horadestino[MAX_PATH],
		tipoavion[MAX_PATH],
		estatus[MAX_PATH], fecharegistro[MAX_PATH];
	char numvueloStr[MAX_PATH], numasientototStr[MAX_PATH], numasientotuStr[MAX_PATH], numasientoejStr[MAX_PATH], menoresStr[MAX_PATH], mayoresStr[MAX_PATH], costotuStr[MAX_PATH], costoejStr[MAX_PATH],
		asientosmenoresStr[MAX_PATH], asientosmayoresStr[MAX_PATH];
	int numvuelo, numasientotot, numasientotu, numasientoej, menores, mayores, costotu, costoej, asientosmenores, asientosmayores;


	switch (msg)
	{

	case WM_INITDIALOG: {
		ListaVuelos.ordenarPorQuicksort();
		// Cargar la nueva imagen del usuario actual
		HBITMAP nuevaImagen = (HBITMAP)LoadImageA(NULL, usuarioActual.RutaImagen, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		if (nuevaImagen != NULL) {
			// Establecer la nueva imagen en el control de imagen correspondiente
			SendMessage(GetDlgItem(hwnd, US_ActualImage), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)nuevaImagen);
		}

		// Obtener el identificador del control de edición US_ActualUser
		HWND editControl = GetDlgItem(hwnd, US_ActualUser);

		if (editControl != NULL) {
			// Establecer el texto del control con el nombre de usuario actual
			SetWindowTextA(editControl, usuarioActual.Username);
		}



		HWND listboxVuelos = GetDlgItem(hwnd, VM_Vuelos);
		if (listboxVuelos != NULL) {
			SendMessage(listboxVuelos, LB_RESETCONTENT, 0, 0);

			NodoVuelo* vuelo = ListaVuelos.cabeza;
			while (vuelo != nullptr) {
				char str[10];
				sprintf_s(str, "%d", vuelo->vuelo.NumVuelo);
				int index = SendMessageA(listboxVuelos, LB_ADDSTRING, 0, (LPARAM)str);
				SendMessage(listboxVuelos, LB_SETITEMDATA, index, (LPARAM)vuelo);
				vuelo = vuelo->siguiente;

			}
		}

		HWND listboxVuelosMan = GetDlgItem(hwnd, VMAN_Vuelos);
		if (listboxVuelosMan != NULL) {
			SendMessage(listboxVuelosMan, LB_RESETCONTENT, 0, 0);

			NodoVuelo* vuelo = ListaVuelos.cabeza;
			while (vuelo != nullptr) {
				char str[10];
				sprintf_s(str, "%d", vuelo->vuelo.NumVuelo);
				int index = SendMessageA(listboxVuelosMan, LB_ADDSTRING, 0, (LPARAM)str);
				SendMessage(listboxVuelosMan, LB_SETITEMDATA, index, (LPARAM)vuelo);
				vuelo = vuelo->siguiente;

			}
		}

		HWND listboxVuelos2 = GetDlgItem(hwnd, VE_Vuelos);
		if (listboxVuelos2 != NULL) {
			SendMessage(listboxVuelos2, LB_RESETCONTENT, 0, 0);

			NodoVuelo* vuelo2 = ListaVuelos.cabeza;
			while (vuelo2 != nullptr) {
				char str2[10];
				sprintf_s(str2, "%d", vuelo2->vuelo.NumVuelo);
				int index2 = SendMessageA(listboxVuelos2, LB_ADDSTRING, 0, (LPARAM)str2);
				SendMessage(listboxVuelos2, LB_SETITEMDATA, index2, (LPARAM)vuelo2);
				vuelo2 = vuelo2->siguiente;

			}
		}

		HWND listboxVuelos3 = GetDlgItem(hwnd, VM_Vuelos2);
		if (listboxVuelos3 != NULL) {
			SendMessage(listboxVuelos3, LB_RESETCONTENT, 0, 0);

			NodoVuelo* vuelo3 = ListaVuelos.cabeza;
			while (vuelo3 != nullptr) {
				char str[10];
				sprintf_s(str, "%d", vuelo3->vuelo.NumVuelo);
				int index = SendMessageA(listboxVuelos3, LB_ADDSTRING, 0, (LPARAM)str);
				SendMessage(listboxVuelos3, LB_SETITEMDATA, index, (LPARAM)vuelo3);
				vuelo3 = vuelo3->siguiente;

			}
		}

		// Llenar el listbox VC_Vuelos3 con vuelos confirmados
		HWND listboxConfirmados = GetDlgItem(hwnd, VC_Vuelos3);
		if (listboxConfirmados != NULL) {
			SendMessage(listboxConfirmados, LB_RESETCONTENT, 0, 0);

			NodoVuelo* vueloConfirmado = ListaVuelos.cabeza;
			while (vueloConfirmado != nullptr) {
				if (strcmp(vueloConfirmado->vuelo.Estatus, "Confirmado") == 0) {
					char str[10];
					sprintf_s(str, "%d", vueloConfirmado->vuelo.NumVuelo);
					int index = SendMessageA(listboxConfirmados, LB_ADDSTRING, 0, (LPARAM)str);
					SendMessage(listboxConfirmados, LB_SETITEMDATA, index, (LPARAM)vueloConfirmado);
				}
				vueloConfirmado = vueloConfirmado->siguiente;
			}
		}

		// Llenar el listbox VC_Vuelos4 con vuelos cancelados
		HWND listboxCancelados = GetDlgItem(hwnd, VC_Vuelos4);
		if (listboxCancelados != NULL) {
			SendMessage(listboxCancelados, LB_RESETCONTENT, 0, 0);

			NodoVuelo* vueloCancelado = ListaVuelos.cabeza;
			while (vueloCancelado != nullptr) {
				if (strcmp(vueloCancelado->vuelo.Estatus, "Cancelado") == 0) {
					char str[10];
					sprintf_s(str, "%d", vueloCancelado->vuelo.NumVuelo);
					int index = SendMessageA(listboxCancelados, LB_ADDSTRING, 0, (LPARAM)str);
					SendMessage(listboxCancelados, LB_SETITEMDATA, index, (LPARAM)vueloCancelado);
				}
				vueloCancelado = vueloCancelado->siguiente;
			}
		}

		HWND listboxVuelos4 = GetDlgItem(hwnd, VS_Vuelos3);
		if (listboxVuelos4 != NULL) {
			SendMessage(listboxVuelos4, LB_RESETCONTENT, 0, 0);

			NodoVuelo* vuelo4 = ListaVuelos.cabeza;
			while (vuelo4 != nullptr) {
				// Agregar el vuelo al ListBox y asociar el puntero al nodo del vuelo
				char str2[10];
				sprintf_s(str2, "%d", vuelo4->vuelo.NumVuelo);
				int index4 = SendMessageA(listboxVuelos4, LB_ADDSTRING, 0, (LPARAM)str2);
				SendMessage(listboxVuelos4, LB_SETITEMDATA, index4, (LPARAM)vuelo4);
				vuelo4 = vuelo4->siguiente;
			}
		}

		HWND listboxVuelos5 = GetDlgItem(hwnd, VS_Vuelos2);
		if (listboxVuelos5 != NULL) {
			SendMessage(listboxVuelos5, LB_RESETCONTENT, 0, 0);

			NodoVuelo* vuelo5 = ListaVuelos.cabeza;
			while (vuelo5 != nullptr) {
				// Agregar el vuelo al ListBox y asociar el puntero al nodo del vuelo
				char str2[10];
				sprintf_s(str2, "%d", vuelo5->vuelo.NumVuelo);
				int index5 = SendMessageA(listboxVuelos5, LB_ADDSTRING, 0, (LPARAM)str2);
				SendMessage(listboxVuelos5, LB_SETITEMDATA, index5, (LPARAM)vuelo5);
				vuelo5 = vuelo5->siguiente;
			}
		}

		// Obtener los handles de los ComboBox de tipo avión
		HWND tipoAvionAgregar = GetDlgItem(hwnd, VR_TipoAvion);
		HWND tipoAvionModificar = GetDlgItem(hwnd, VM_TipoAvion);
		HWND tipoAvionEliminar = GetDlgItem(hwnd, VE_TipoAvion);
		HWND tipoAvionManifiesto = GetDlgItem(hwnd, VS_TipoAvion);
		HWND lugarOrigen = GetDlgItem(hwnd, VR_Origen);
		HWND lugarDestino = GetDlgItem(hwnd, VR_Destino);
		HWND lugarOrigen2 = GetDlgItem(hwnd, VM_Origen2);
		HWND lugarDestino2 = GetDlgItem(hwnd, VM_Destino2);

		// Agregar elementos al ComboBox
		SendMessage(tipoAvionAgregar, CB_ADDSTRING, 0, (LPARAM)L"Tipo A");
		SendMessage(tipoAvionAgregar, CB_ADDSTRING, 0, (LPARAM)L"Tipo B");
		SendMessage(tipoAvionAgregar, CB_ADDSTRING, 0, (LPARAM)L"Tipo C");

		// Agregar elementos al ComboBox
		SendMessage(tipoAvionModificar, CB_ADDSTRING, 0, (LPARAM)L"Tipo A");
		SendMessage(tipoAvionModificar, CB_ADDSTRING, 0, (LPARAM)L"Tipo B");
		SendMessage(tipoAvionModificar, CB_ADDSTRING, 0, (LPARAM)L"Tipo C");

		// Agregar elementos al ComboBox
		SendMessage(tipoAvionEliminar, CB_ADDSTRING, 0, (LPARAM)L"Tipo A");
		SendMessage(tipoAvionEliminar, CB_ADDSTRING, 0, (LPARAM)L"Tipo B");
		SendMessage(tipoAvionEliminar, CB_ADDSTRING, 0, (LPARAM)L"Tipo C");

		// Agregar elementos al ComboBox
		SendMessage(tipoAvionManifiesto, CB_ADDSTRING, 0, (LPARAM)L"Tipo A");
		SendMessage(tipoAvionManifiesto, CB_ADDSTRING, 0, (LPARAM)L"Tipo B");
		SendMessage(tipoAvionManifiesto, CB_ADDSTRING, 0, (LPARAM)L"Tipo C");

		// Agregar elementos al ComboBox
		SendMessage(lugarOrigen, CB_ADDSTRING, 0, (LPARAM)L"Monterrey");
		SendMessage(lugarOrigen, CB_ADDSTRING, 0, (LPARAM)L"Guadalajara");
		SendMessage(lugarOrigen, CB_ADDSTRING, 0, (LPARAM)L"Guanajuato");

		// Agregar elementos al ComboBox
		SendMessage(lugarDestino, CB_ADDSTRING, 0, (LPARAM)L"Monterrey");
		SendMessage(lugarDestino, CB_ADDSTRING, 0, (LPARAM)L"Guadalajara");
		SendMessage(lugarDestino, CB_ADDSTRING, 0, (LPARAM)L"Guanajuato");

		// Agregar elementos al ComboBox
		SendMessage(lugarOrigen2, CB_ADDSTRING, 0, (LPARAM)L"Monterrey");
		SendMessage(lugarOrigen2, CB_ADDSTRING, 0, (LPARAM)L"Guadalajara");
		SendMessage(lugarOrigen2, CB_ADDSTRING, 0, (LPARAM)L"Guanajuato");

		// Agregar elementos al ComboBox
		SendMessage(lugarDestino2, CB_ADDSTRING, 0, (LPARAM)L"Monterrey");
		SendMessage(lugarDestino2, CB_ADDSTRING, 0, (LPARAM)L"Guadalajara");
		SendMessage(lugarDestino2, CB_ADDSTRING, 0, (LPARAM)L"Guanajuato");

	}break;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		if (menu(wmId, hwnd))
			return FALSE;

		switch (wmId)
		{
		case VR_Generar: {
			Random = rand() % 99 + 1; // Genera un número aleatorio entre 1 y 99
			do {
				SetDlgItemInt(hwnd, VR_NumVuelo, Random, FALSE);
			} while (numerosUsados.find(Random) != numerosUsados.end()); //revisa si ya existe 


		}break;

		case VR_Generar2: {
			//genera los asientos de ejecutivo y turista a partir de asientos totales
			int Turista, Ejecutivos, Totales;
			float  porcentajeTurista = 0.75;
			GetWindowTextA(GetDlgItem(hwnd, VR_NumAsientosTot), numasientototStr, MAX_PATH);
			Totales = atoi(numasientototStr);
			Turista = round(Totales * porcentajeTurista);
			Ejecutivos = Totales - Turista;
			SetDlgItemInt(hwnd, VR_NumAsientoEj, Ejecutivos, FALSE);
			SetDlgItemInt(hwnd, VR_NumAsientoEj2, Turista, FALSE);

		}break;

		case VR_Generar3: {
			//genera los porcentajes de los asientos menores y mayores
			int PorcentajeMEN, PorcentajeMAY, AsientosMEN, AsientosMAY, Totales;

			GetWindowTextA(GetDlgItem(hwnd, VR_Menores), menoresStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_Mayores), mayoresStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_NumAsientosTot), numasientototStr, MAX_PATH);
			PorcentajeMEN = atoi(menoresStr);
			PorcentajeMAY = atoi(mayoresStr);
			Totales = atoi(numasientototStr);
			AsientosMEN = Totales * PorcentajeMEN / 100;
			AsientosMAY = Totales * PorcentajeMAY / 100;

			SetDlgItemInt(hwnd, VR_Menores2, AsientosMEN, FALSE);
			SetDlgItemInt(hwnd, VR_Mayores2, AsientosMAY, FALSE);

		}break;

		case VR_Guardar: {
			numerosUsados.insert(Random);

			GetWindowTextA(GetDlgItem(hwnd, VR_Origen), origen, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_Destino), destino, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_NumVuelo), numvueloStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_TipoAvion), tipoavion, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_NumAsientosTot), numasientototStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_NumAsientoEj), numasientoejStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_NumAsientoEj2), numasientotuStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_Menores), menoresStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_Mayores), mayoresStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_Menores2), asientosmenoresStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_Mayores2), asientosmayoresStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_CostoTu), costotuStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_CostoEj), costoejStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VR_Estatus), estatus, MAX_PATH);

			numvuelo = atoi(numvueloStr);
			numasientotot = atoi(numasientototStr);
			numasientoej = atoi(numasientoejStr);
			numasientotu = atoi(numasientotuStr);
			menores = atoi(menoresStr);
			mayores = atoi(mayoresStr);
			asientosmenores = atoi(asientosmenoresStr);
			asientosmayores = atoi(asientosmayoresStr);
			costotu = atoi(costotuStr);
			costoej = atoi(costoejStr);

			SYSTEMTIME FechaRegistro;
			SendMessage(GetDlgItem(hwnd, VR_FechaRegistro), DTM_GETSYSTEMTIME, 0, (LPARAM)&FechaRegistro);
			GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &FechaRegistro, NULL, fecharegistro, MAX_PATH);

			SYSTEMTIME FechaOrigen;
			SendMessage(GetDlgItem(hwnd, VR_FechaOrigen), DTM_GETSYSTEMTIME, 0, (LPARAM)&FechaOrigen);
			GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &FechaOrigen, NULL, fechaorigen, MAX_PATH);

			SYSTEMTIME HoraOrigen;
			SendMessage(GetDlgItem(hwnd, VR_HoraOrigen), DTM_GETSYSTEMTIME, 0, (LPARAM)&HoraOrigen);
			char horaorigen[64]; // Asegúrate de tener suficiente espacio
			GetTimeFormatA(LOCALE_USER_DEFAULT, 0, &HoraOrigen, NULL, horaorigen, sizeof(horaorigen));

			SYSTEMTIME FechaDestino;
			SendMessage(GetDlgItem(hwnd, VR_FechaDestino), DTM_GETSYSTEMTIME, 0, (LPARAM)&FechaDestino);
			GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &FechaDestino, NULL, fechadestino, MAX_PATH);

			SYSTEMTIME HoraDestino;
			SendMessage(GetDlgItem(hwnd, VR_HoraDestino), DTM_GETSYSTEMTIME, 0, (LPARAM)&HoraDestino);
			char horadestino[64]; // Asegúrate de tener suficiente espacio
			GetTimeFormatA(LOCALE_USER_DEFAULT, 0, &HoraDestino, NULL, horadestino, sizeof(horadestino));

			ListaVuelos.ordenarPorQuicksort();
			NodoVuelo* VueloExistente = ListaVuelos.buscarBinario(numvuelo);
			if (VueloExistente != nullptr) {
				// El usuario ya existe, mostrar mensaje de error
				MessageBoxA(hwnd, "Este numero de vuelo ha sido asignado ya.", "Error", MB_OK | MB_ICONERROR);
			}
			else {
				if (strlen(origen) == 0 || strlen(destino) == 0 || strlen(fechaorigen) == 0 || strlen(horaorigen) == 0 ||
					strlen(fechadestino) == 0 || strlen(horadestino) == 0 || numvuelo == 0 || strlen(tipoavion) == 0 ||
					numasientotot == 0 || numasientotu == 0 || numasientoej == 0 || menores == 0 || mayores == 0 ||
					costotu == 0 || costoej == 0 || strlen(fecharegistro) == 0 || asientosmenores == 0 || asientosmayores == 0) {
					// Mostrar un mensaje de advertencia si algún campo está vacío
					MessageBoxA(hwnd, "Todos los campos deben completarse.", "Advertencia", MB_OK | MB_ICONWARNING);
				}
				else {

					Vuelo nuevoVuelo;
					strcpy_s(nuevoVuelo.Origen, origen);
					strcpy_s(nuevoVuelo.Destino, destino);
					strcpy_s(nuevoVuelo.FechaOrigen, fechaorigen);
					strcpy_s(nuevoVuelo.HoraOrigen, horaorigen);
					strcpy_s(nuevoVuelo.FechaDestino, fechadestino);
					strcpy_s(nuevoVuelo.HoraDestino, horadestino);
					strcpy_s(nuevoVuelo.TipoAvion, tipoavion);
					strcpy_s(nuevoVuelo.FechaRegistro, fecharegistro);
					nuevoVuelo.NumVuelo = numvuelo;
					nuevoVuelo.NumAsientosTot = numasientotot;
					nuevoVuelo.NumAsientoTu = numasientotu;
					nuevoVuelo.NumAsientoEj = numasientoej;
					nuevoVuelo.Menores = menores;
					nuevoVuelo.Mayores = mayores;
					nuevoVuelo.CostoTu = costotu;
					nuevoVuelo.CostoEj = costoej;
					nuevoVuelo.asientosMenores = asientosmenores;
					nuevoVuelo.asientosMayores = asientosmayores;

					ListaAsientos.inicializar_asientos(nuevoVuelo.NumVuelo);
					ListaVuelos.agregarAlFinal(nuevoVuelo);

					ListaVuelos.guardar(rutaGeneral2);
					ListaAsientos.guardar(rutaGeneral4);
					//ListaVuelos.inicializar_asientos(VueloExistente->listaAsientosDisponibles);

					MessageBoxA(NULL, "Vuelo registrado correctamente", "Vuelos", MB_OK | MB_ICONINFORMATION);

				}
			}
		} break;
		case VM_Generar2: {
			//genera los asientos de ejecutivo y turista a partir de asientos totales
			int Turista, Ejecutivos, Totales;
			float  porcentajeTurista = 0.75;
			GetWindowTextA(GetDlgItem(hwnd, VM_NumAsientoTot), numasientototStr, MAX_PATH);
			Totales = atoi(numasientototStr);
			Turista = round(Totales * porcentajeTurista);
			Ejecutivos = Totales - Turista;
			SetDlgItemInt(hwnd, VM_NumAsientoEj, Ejecutivos, FALSE);
			SetDlgItemInt(hwnd, VM_NumAsientoEj2, Turista, FALSE);

		}break;

		case VM_Generar3: {
			//genera los porcentajes de los asientos menores y mayores
			int PorcentajeMEN, PorcentajeMAY, AsientosMEN, AsientosMAY, Totales;

			GetWindowTextA(GetDlgItem(hwnd, VM_Menores), menoresStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VM_Mayores), mayoresStr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, VM_NumAsientoTot), numasientototStr, MAX_PATH);
			PorcentajeMEN = atoi(menoresStr);
			PorcentajeMAY = atoi(mayoresStr);
			Totales = atoi(numasientototStr);
			AsientosMEN = Totales * PorcentajeMEN / 100;
			AsientosMAY = Totales * PorcentajeMAY / 100;

			SetDlgItemInt(hwnd, VM_Menores2, AsientosMEN, FALSE);
			SetDlgItemInt(hwnd, VM_Mayores2, AsientosMAY, FALSE);

		}break;

		case VM_Vuelos: {
			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxVuelos = GetDlgItem(hwnd, VM_Vuelos);
				if (listBoxVuelos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoVuelo* vueloSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
						if (vueloSeleccionado != nullptr) {

							char Vuelostr[10];
							sprintf_s(Vuelostr, "%d", vueloSeleccionado->vuelo.NumVuelo);
							char Totalesstr[10];
							sprintf_s(Totalesstr, "%d", vueloSeleccionado->vuelo.NumAsientosTot);

							SetWindowTextA(GetDlgItem(hwnd, VC_Origen), vueloSeleccionado->vuelo.Origen);
							SetWindowTextA(GetDlgItem(hwnd, VC_Destino), vueloSeleccionado->vuelo.Destino);
							SetWindowTextA(GetDlgItem(hwnd, VC_FechaOrigen), vueloSeleccionado->vuelo.FechaOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VC_HoraOrigen), vueloSeleccionado->vuelo.HoraOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VC_FechaDestino), vueloSeleccionado->vuelo.FechaDestino);
							SetWindowTextA(GetDlgItem(hwnd, VC_HoraDestino), vueloSeleccionado->vuelo.HoraDestino);
							SetWindowTextA(GetDlgItem(hwnd, VC_NumVuelo), Vuelostr);
							SetWindowTextA(GetDlgItem(hwnd, VC_NumPasajeros), Totalesstr);


						}
					}
				}
			}
			break;
		}break;
		case VMAN_Vuelos: {
			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxVuelos = GetDlgItem(hwnd, VMAN_Vuelos);
				if (listBoxVuelos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoVuelo* vueloSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
						if (vueloSeleccionado != nullptr) {

							char Vuelostr[10];
							sprintf_s(Vuelostr, "%d", vueloSeleccionado->vuelo.NumVuelo);
							char Totalesstr[10];
							sprintf_s(Totalesstr, "%d", vueloSeleccionado->vuelo.NumAsientosTot);

							SetWindowTextA(GetDlgItem(hwnd, VS_Origen), vueloSeleccionado->vuelo.Origen);
							SetWindowTextA(GetDlgItem(hwnd, VS_Destino), vueloSeleccionado->vuelo.Destino);
							SetWindowTextA(GetDlgItem(hwnd, VS_FechaOrigen), vueloSeleccionado->vuelo.FechaOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VS_HoraOrigen), vueloSeleccionado->vuelo.HoraOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VS_NumVuelo), Vuelostr);
							SetWindowTextA(GetDlgItem(hwnd, VS_TipoAvion), vueloSeleccionado->vuelo.TipoAvion);

							HWND listboxBoleto = GetDlgItem(hwnd, VS_Pasajeros);
							if (listboxBoleto != NULL) {
								SendMessage(listboxBoleto, LB_RESETCONTENT, 0, 0);

								NodoBoleto* Aletra = ListaBoletos.cabeza;
								while (Aletra != nullptr) {
									if (Aletra->boleto.numVuelo == vueloSeleccionado->vuelo.NumVuelo) {
										// Incluir el estatus en la cadena de visualización
										char str[300]; // Asegúrate de que el tamaño del array sea suficiente para contener la cadena completa
										sprintf_s(str, "%c%d - %s %s %s - %s",
											Aletra->boleto.Aletra,
											Aletra->boleto.Anumero,
											Aletra->boleto.NombrePasajero,
											Aletra->boleto.ApellidoP,
											Aletra->boleto.ApellidoM,
											Aletra->boleto.estatus);
										int index = SendMessageA(listboxBoleto, LB_ADDSTRING, 0, (LPARAM)str);
										SendMessage(listboxBoleto, LB_SETITEMDATA, index, (LPARAM)Aletra);
									}
									Aletra = Aletra->siguiente;
								}
							}


						}
					}
				}
			}
			break;
		}break;
		case VM_Vuelos2: {

			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxVuelos = GetDlgItem(hwnd, VM_Vuelos2);
				if (listBoxVuelos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoVuelo* vueloSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
						if (vueloSeleccionado != nullptr) {

							char Vuelostr[10];
							sprintf_s(Vuelostr, "%d", vueloSeleccionado->vuelo.NumVuelo);
							char Totalesstr[10];
							sprintf_s(Totalesstr, "%d", vueloSeleccionado->vuelo.NumAsientosTot);
							char Ejecutivosstr[10];
							sprintf_s(Ejecutivosstr, "%d", vueloSeleccionado->vuelo.NumAsientoEj);
							char Turistasstr[10];
							sprintf_s(Turistasstr, "%d", vueloSeleccionado->vuelo.NumAsientoTu);
							char Pmenoresstr[10];
							sprintf_s(Pmenoresstr, "%d", vueloSeleccionado->vuelo.Menores);
							char Pmayoresstr[10];
							sprintf_s(Pmayoresstr, "%d", vueloSeleccionado->vuelo.Mayores);
							char Amenoresstr[10];
							sprintf_s(Amenoresstr, "%d", vueloSeleccionado->vuelo.asientosMenores);
							char Amayoresstr[10];
							sprintf_s(Amayoresstr, "%d", vueloSeleccionado->vuelo.asientosMayores);
							char Cejestr[10];
							sprintf_s(Cejestr, "%d", vueloSeleccionado->vuelo.CostoEj);
							char Cturstr[10];
							sprintf_s(Cturstr, "%d", vueloSeleccionado->vuelo.CostoTu);

							SetWindowTextA(GetDlgItem(hwnd, VM_Origen), vueloSeleccionado->vuelo.Origen);
							SetWindowTextA(GetDlgItem(hwnd, VM_Destino), vueloSeleccionado->vuelo.Destino);
							SetWindowTextA(GetDlgItem(hwnd, VM_FechaOrigen2), vueloSeleccionado->vuelo.FechaOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VM_HoraOrigen2), vueloSeleccionado->vuelo.HoraOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VM_NumVuelo), Vuelostr);
							SetWindowTextA(GetDlgItem(hwnd, VM_TipoAvion2), vueloSeleccionado->vuelo.TipoAvion);
							SetWindowTextA(GetDlgItem(hwnd, VM_NumAsientoTot), Totalesstr);
							SetWindowTextA(GetDlgItem(hwnd, VM_NumAsientoEj), Ejecutivosstr);
							SetWindowTextA(GetDlgItem(hwnd, VM_NumAsientoEj2), Turistasstr);
							SetWindowTextA(GetDlgItem(hwnd, VM_Menores), Pmenoresstr);
							SetWindowTextA(GetDlgItem(hwnd, VM_Mayores), Pmayoresstr);
							SetWindowTextA(GetDlgItem(hwnd, VM_Menores2), Amenoresstr);
							SetWindowTextA(GetDlgItem(hwnd, VM_Mayores2), Amayoresstr);
							SetWindowTextA(GetDlgItem(hwnd, VM_CostoEj), Cejestr);
							SetWindowTextA(GetDlgItem(hwnd, VM_CostoTu), Cturstr);

						}
					}
				}
			}
			break;
		}break;
		case VS_Vuelos2: {
			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxVuelos = GetDlgItem(hwnd, VS_Vuelos2);
				if (listBoxVuelos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoVuelo* vueloSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
						if (vueloSeleccionado != nullptr) {

							char Vuelostr[10];
							sprintf_s(Vuelostr, "%d", vueloSeleccionado->vuelo.NumVuelo);

							SetWindowTextA(GetDlgItem(hwnd, VREP_FechaOrigen), vueloSeleccionado->vuelo.FechaOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VREP_HoraOrigen), vueloSeleccionado->vuelo.HoraOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VREP_Origen), vueloSeleccionado->vuelo.Origen);
							SetWindowTextA(GetDlgItem(hwnd, VREP_Destinos), vueloSeleccionado->vuelo.Destino);
							SetWindowTextA(GetDlgItem(hwnd, VREP_NumVuelo), Vuelostr);
						}
					}
				}
			}
			break;
		}break;
		case VS_Vuelos3: {

			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxVuelos = GetDlgItem(hwnd, VS_Vuelos3);
				if (listBoxVuelos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoVuelo* vueloSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
						if (vueloSeleccionado != nullptr) {

							char Vuelostr[10];
							sprintf_s(Vuelostr, "%d", vueloSeleccionado->vuelo.NumVuelo);

							SetWindowTextA(GetDlgItem(hwnd, VREP_FechaOrigen), vueloSeleccionado->vuelo.FechaOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VREP_HoraOrigen), vueloSeleccionado->vuelo.HoraOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VREP_Origen), vueloSeleccionado->vuelo.Origen);
							SetWindowTextA(GetDlgItem(hwnd, VREP_Destinos), vueloSeleccionado->vuelo.Destino);
							SetWindowTextA(GetDlgItem(hwnd, VREP_NumVuelo), Vuelostr);
						}
					}
				}
			}
			break;
		}break;

		case VM_Modificar: {
			// Obtener el handle del ListBox
			HWND listBoxVuelos = GetDlgItem(hwnd, VM_Vuelos2);
			if (listBoxVuelos != NULL) {
				// Obtener el índice del elemento seleccionado
				int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
				if (selectedIndex != LB_ERR) {
					// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
					NodoVuelo* vueloSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
					if (vueloSeleccionado != nullptr) {
						char nuevoorigen[100], nuevodestino[100], nuevofechaorigen[100], nuevohoraorigen[100], nuevofechadestino[100], nuevohoradestino[100],
							nuevotipoavion[100], nuevoestatus[100], nuevofecharegistro[100];
						char nuevonumvueloStr[100], nuevonumasientototStr[100], nuevonumasientotuStr[100], nuevonumasientoejStr[100], nuevomenoresStr[100], nuevomayoresStr[100], nuevocostotuStr[100], nuevocostoejStr[100],
							nuevoasientosmenoresStr[100], nuevoasientosmayoresStr[100];
						int nuevonumvuelo, nuevonumasientotot, nuevonumasientotu, nuevonumasientoej, nuevomenores, nuevomayores, nuevocostotu, nuevocostoej, nuevoasientosmenores, nuevoasientosmayores;

						GetWindowTextA(GetDlgItem(hwnd, VM_Origen2), nuevoorigen, sizeof(nuevoorigen));
						GetWindowTextA(GetDlgItem(hwnd, VM_Destino2), nuevodestino, sizeof(nuevodestino));
						GetWindowTextA(GetDlgItem(hwnd, VM_NumVuelo), nuevonumvueloStr, sizeof(nuevonumvueloStr));
						GetWindowTextA(GetDlgItem(hwnd, VM_TipoAvion), nuevotipoavion, sizeof(nuevotipoavion));
						GetWindowTextA(GetDlgItem(hwnd, VM_NumAsientoTot), nuevonumasientototStr, sizeof(nuevonumasientototStr));
						GetWindowTextA(GetDlgItem(hwnd, VM_NumAsientoEj), nuevonumasientoejStr, sizeof(nuevonumasientoejStr));
						GetWindowTextA(GetDlgItem(hwnd, VM_NumAsientoEj2), nuevonumasientotuStr, sizeof(nuevonumasientotuStr));
						GetWindowTextA(GetDlgItem(hwnd, VM_Menores), nuevomenoresStr, sizeof(nuevomenoresStr));
						GetWindowTextA(GetDlgItem(hwnd, VM_Mayores), nuevomayoresStr, sizeof(nuevomayoresStr));
						GetWindowTextA(GetDlgItem(hwnd, VM_Menores2), nuevoasientosmenoresStr, sizeof(nuevoasientosmenoresStr));
						GetWindowTextA(GetDlgItem(hwnd, VM_Mayores2), nuevoasientosmayoresStr, sizeof(nuevoasientosmayoresStr));
						GetWindowTextA(GetDlgItem(hwnd, VM_CostoTu), nuevocostotuStr, sizeof(nuevocostotuStr));
						GetWindowTextA(GetDlgItem(hwnd, VM_CostoEj), nuevocostoejStr, sizeof(nuevocostoejStr));
						//GetWindowTextA(GetDlgItem(hwnd, VM_Estatus), estatus, MAX_PATH);
						nuevonumvuelo = atoi(nuevonumvueloStr);
						nuevonumasientotot = atoi(nuevonumasientototStr);
						nuevonumasientoej = atoi(nuevonumasientoejStr);
						nuevonumasientotu = atoi(nuevonumasientotuStr);
						nuevomenores = atoi(nuevomenoresStr);
						nuevomayores = atoi(nuevomayoresStr);
						nuevoasientosmenores = atoi(nuevoasientosmenoresStr);
						nuevoasientosmayores = atoi(nuevoasientosmayoresStr);
						nuevocostotu = atoi(nuevocostotuStr);
						nuevocostoej = atoi(nuevocostoejStr);

						SYSTEMTIME nuevoFechaRegistro;
						SendMessage(GetDlgItem(hwnd, VM_FechaRegistro), DTM_GETSYSTEMTIME, 0, (LPARAM)&nuevoFechaRegistro);
						GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &nuevoFechaRegistro, NULL, nuevofecharegistro, MAX_PATH);

						SYSTEMTIME nuevoFechaOrigen;
						SendMessage(GetDlgItem(hwnd, VM_FechaOrigen), DTM_GETSYSTEMTIME, 0, (LPARAM)&nuevoFechaOrigen);
						GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &nuevoFechaOrigen, NULL, nuevofechaorigen, MAX_PATH);

						// Obtener la hora seleccionada del control DateTimePicker
						SYSTEMTIME nuevoHoraOrigen;
						SendMessage(GetDlgItem(hwnd, VM_HoraOrigen), DTM_GETSYSTEMTIME, 0, (LPARAM)&nuevoHoraOrigen);
						// Formatear la hora obtenida en el formato deseado
						GetTimeFormatA(LOCALE_USER_DEFAULT, 0, &nuevoHoraOrigen, NULL, nuevohoraorigen, sizeof(nuevohoraorigen));

						Vuelo nuevoVuelo;
						strcpy_s(nuevoVuelo.Origen, nuevoorigen);
						strcpy_s(nuevoVuelo.Destino, nuevodestino);
						strcpy_s(nuevoVuelo.FechaOrigen, nuevofechaorigen);
						strcpy_s(nuevoVuelo.HoraOrigen, nuevohoraorigen);
						strcpy_s(nuevoVuelo.TipoAvion, nuevotipoavion);
						strcpy_s(nuevoVuelo.FechaRegistro, nuevofecharegistro);
						nuevoVuelo.NumVuelo = nuevonumvuelo;
						nuevoVuelo.NumAsientosTot = nuevonumasientotot;
						nuevoVuelo.NumAsientoTu = nuevonumasientotu;
						nuevoVuelo.NumAsientoEj = nuevonumasientoej;
						nuevoVuelo.Menores = nuevomenores;
						nuevoVuelo.Mayores = nuevomayores;
						nuevoVuelo.CostoTu = nuevocostotu;
						nuevoVuelo.CostoEj = nuevocostoej;
						nuevoVuelo.asientosMenores = nuevoasientosmenores;
						nuevoVuelo.asientosMayores = nuevoasientosmayores;

						ListaVuelos.modificarNodo(vueloSeleccionado, nuevoVuelo);
						ListaVuelos.guardar(rutaGeneral2);
						MessageBox(hwnd, L"Datos actualizados con exito.", L"Exito", MB_OK | MB_ICONINFORMATION);
					}
				}
			}
		}break;

		case VE_Vuelos:
			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxVuelos = GetDlgItem(hwnd, VE_Vuelos);
				if (listBoxVuelos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoVuelo* vueloSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
						if (vueloSeleccionado != nullptr) {
							char Vuelostr[10];
							sprintf_s(Vuelostr, "%d", vueloSeleccionado->vuelo.NumVuelo);
							char Totalesstr[10];
							sprintf_s(Totalesstr, "%d", vueloSeleccionado->vuelo.NumAsientosTot);
							char Ejecutivosstr[10];
							sprintf_s(Ejecutivosstr, "%d", vueloSeleccionado->vuelo.NumAsientoEj);
							char Turistasstr[10];
							sprintf_s(Turistasstr, "%d", vueloSeleccionado->vuelo.NumAsientoTu);
							char Pmenoresstr[10];
							sprintf_s(Pmenoresstr, "%d", vueloSeleccionado->vuelo.Menores);
							char Pmayoresstr[10];
							sprintf_s(Pmayoresstr, "%d", vueloSeleccionado->vuelo.Mayores);
							char Amenoresstr[10];
							sprintf_s(Amenoresstr, "%d", vueloSeleccionado->vuelo.asientosMenores);
							char Amayoresstr[10];
							sprintf_s(Amayoresstr, "%d", vueloSeleccionado->vuelo.asientosMayores);
							char Cejestr[10];
							sprintf_s(Cejestr, "%d", vueloSeleccionado->vuelo.CostoEj);
							char Cturstr[10];
							sprintf_s(Cturstr, "%d", vueloSeleccionado->vuelo.CostoTu);

							SetWindowTextA(GetDlgItem(hwnd, VE_Origen), vueloSeleccionado->vuelo.Origen);
							SetWindowTextA(GetDlgItem(hwnd, VE_Destino), vueloSeleccionado->vuelo.Destino);
							SetWindowTextA(GetDlgItem(hwnd, VE_FechaOrigen), vueloSeleccionado->vuelo.FechaOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VE_HoraOrigen), vueloSeleccionado->vuelo.HoraOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VE_FechaDestino), vueloSeleccionado->vuelo.FechaDestino);
							SetWindowTextA(GetDlgItem(hwnd, VE_HoraDestino), vueloSeleccionado->vuelo.HoraDestino);
							SetWindowTextA(GetDlgItem(hwnd, VE_NumVuelo), Vuelostr);
							SetWindowTextA(GetDlgItem(hwnd, VE_TipoAvion), vueloSeleccionado->vuelo.TipoAvion);
							SetWindowTextA(GetDlgItem(hwnd, VE_NumAsientosTot), Totalesstr);
							SetWindowTextA(GetDlgItem(hwnd, VE_NumAsientoEj), Ejecutivosstr);
							SetWindowTextA(GetDlgItem(hwnd, VE_NumAsientoEj2), Turistasstr);
							SetWindowTextA(GetDlgItem(hwnd, VE_Menores), Pmenoresstr);
							SetWindowTextA(GetDlgItem(hwnd, VE_Mayores), Pmayoresstr);
							SetWindowTextA(GetDlgItem(hwnd, VE_Menores2), Amenoresstr);
							SetWindowTextA(GetDlgItem(hwnd, VE_Mayores2), Amayoresstr);
							SetWindowTextA(GetDlgItem(hwnd, VE_CostoEj), Cejestr);
							SetWindowTextA(GetDlgItem(hwnd, VE_CostoTu), Cturstr);
						}

					}
				}
			}
			break;

		case VE_Eliminar: {
			// Obtener el handle del ListBox RP_Pasajero
			HWND listBoxVuelos = GetDlgItem(hwnd, VE_Vuelos);
			if (listBoxVuelos != NULL) {
				// Obtener el índice del elemento seleccionado
				int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
				if (selectedIndex != LB_ERR) {
					// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
					NodoVuelo* VuelosSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
					if (VuelosSeleccionado != nullptr) {
						// Mostrar un mensaje de confirmación
						int result = MessageBox(hwnd, L"Estas seguro de que quieres eliminar este vuelo?", L"Confirmar eliminacion", MB_YESNO | MB_ICONQUESTION);
						if (result == IDYES) {
							auto it = numerosUsados.find(VuelosSeleccionado->vuelo.NumVuelo);
							if (it != numerosUsados.end()) {
								numerosUsados.erase(it); // Elimina el número de los utilizados
							}
							ListaVuelos.eliminarNodo(VuelosSeleccionado);
							ListaVuelos.guardar(rutaGeneral2);
							MessageBox(hwnd, L"Pasajero eliminado con exito.", L"Exito", MB_OK | MB_ICONINFORMATION);
						}
					}
				}
			}
		}break;

		case VREP_Generar: {
			bool exito = guardarListasEnArchivo(ListaVuelos, ListaAsientos, "ReporteVuelos.txt");
			if (exito) {
				MessageBox(hwnd, L"Reporte de vuelos generado y guardado exitosamente.", L"Confirmacion", MB_OK | MB_ICONINFORMATION);
			}
			else {
				MessageBox(hwnd, L"Error al generar el reporte de vuelos.", L"Error", MB_OK | MB_ICONERROR);
			}
		} break;

		case VC_Confirmar: {
			char numvueloStr[MAX_PATH];
			GetWindowTextA(GetDlgItem(hwnd, VC_NumVuelo), numvueloStr, MAX_PATH);
			int numvuelo = atoi(numvueloStr);

			NodoVuelo* vueloEncontrado = ListaVuelos.buscar(numvuelo);

			if (vueloEncontrado != nullptr) {
				strcpy_s(vueloEncontrado->vuelo.Estatus, "Confirmado");
				ListaVuelos.guardar(rutaGeneral2);
				MessageBoxA(hwnd, "El estado del vuelo ha sido actualizado a Confirmado.", "Informacion", MB_OK | MB_ICONINFORMATION);
			}
			else {
				MessageBoxA(hwnd, "No se encontró el vuelo con el numero especificado.", "Error", MB_OK | MB_ICONERROR);
			}
		} break;

		case VC_Cancelar: {
			char numvueloStr[MAX_PATH];
			GetWindowTextA(GetDlgItem(hwnd, VC_NumVuelo), numvueloStr, MAX_PATH);
			int numvuelo = atoi(numvueloStr);

			NodoVuelo* vueloEncontrado = ListaVuelos.buscar(numvuelo);

			if (vueloEncontrado != nullptr) {
				strcpy_s(vueloEncontrado->vuelo.Estatus, "Cancelado");
				ListaVuelos.guardar(rutaGeneral2);
				MessageBoxA(hwnd, "El estado del vuelo ha sido actualizado a Cancelado.", "Informacion", MB_OK | MB_ICONINFORMATION);
			}
			else {
				MessageBoxA(hwnd, "No se encontro el vuelo con el numero especificado.", "Error", MB_OK | MB_ICONERROR);
			}
		} break;

		case VC_Vuelos3: {
			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxVuelos = GetDlgItem(hwnd, VC_Vuelos3);
				if (listBoxVuelos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoVuelo* vueloSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
						if (vueloSeleccionado != nullptr) {

							char Vuelostr[10];
							sprintf_s(Vuelostr, "%d", vueloSeleccionado->vuelo.NumVuelo);
							char Totalesstr[10];
							sprintf_s(Totalesstr, "%d", vueloSeleccionado->vuelo.NumAsientosTot);

							SetWindowTextA(GetDlgItem(hwnd, VC_Origen), vueloSeleccionado->vuelo.Origen);
							SetWindowTextA(GetDlgItem(hwnd, VC_Destino), vueloSeleccionado->vuelo.Destino);
							SetWindowTextA(GetDlgItem(hwnd, VC_FechaOrigen), vueloSeleccionado->vuelo.FechaOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VC_HoraOrigen), vueloSeleccionado->vuelo.HoraOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VC_FechaDestino), vueloSeleccionado->vuelo.FechaDestino);
							SetWindowTextA(GetDlgItem(hwnd, VC_HoraDestino), vueloSeleccionado->vuelo.HoraDestino);
							SetWindowTextA(GetDlgItem(hwnd, VC_NumVuelo), Vuelostr);
							SetWindowTextA(GetDlgItem(hwnd, VC_NumPasajeros), Totalesstr);

						}
					}
				}
			}
			break;
		} break;
		case VC_Vuelos4: {
			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxVuelos = GetDlgItem(hwnd, VC_Vuelos4);
				if (listBoxVuelos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoVuelo* vueloSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
						if (vueloSeleccionado != nullptr) {

							char Vuelostr[10];
							sprintf_s(Vuelostr, "%d", vueloSeleccionado->vuelo.NumVuelo);
							char Totalesstr[10];
							sprintf_s(Totalesstr, "%d", vueloSeleccionado->vuelo.NumAsientosTot);

							SetWindowTextA(GetDlgItem(hwnd, VC_Origen), vueloSeleccionado->vuelo.Origen);
							SetWindowTextA(GetDlgItem(hwnd, VC_Destino), vueloSeleccionado->vuelo.Destino);
							SetWindowTextA(GetDlgItem(hwnd, VC_FechaOrigen), vueloSeleccionado->vuelo.FechaOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VC_HoraOrigen), vueloSeleccionado->vuelo.HoraOrigen);
							SetWindowTextA(GetDlgItem(hwnd, VC_FechaDestino), vueloSeleccionado->vuelo.FechaDestino);
							SetWindowTextA(GetDlgItem(hwnd, VC_HoraDestino), vueloSeleccionado->vuelo.HoraDestino);
							SetWindowTextA(GetDlgItem(hwnd, VC_NumVuelo), Vuelostr);
							SetWindowTextA(GetDlgItem(hwnd, VC_NumPasajeros), Totalesstr);

						}
					}
				}
			}
			break;
		} break;
		case VREP_Aceptar: {
			// Limpiar el listbox
			SendMessage(GetDlgItem(hwnd, VS_Vuelos2), LB_RESETCONTENT, 0, 0);

			// Obtener las fechas seleccionadas en los DateTimePickers
			SYSTEMTIME fechaInicio, fechaFinal;
			SendMessage(GetDlgItem(hwnd, VREP_FiltroInicio), DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaInicio);
			SendMessage(GetDlgItem(hwnd, VREP_FiltroFinal), DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaFinal);

			// Convertir las fechas seleccionadas a cadenas de caracteres
			char fechaInicioStr[MAX_PATH];
			char fechaFinalStr[MAX_PATH];
			GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &fechaInicio, NULL, fechaInicioStr, MAX_PATH);
			GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &fechaFinal, NULL, fechaFinalStr, MAX_PATH);

			// Iterar sobre la lista de vuelos y filtrar por fechas
			NodoVuelo* vuelo = ListaVuelos.cabeza;
			while (vuelo != nullptr) {
				// Obtener la fecha de origen del vuelo
				const char* fechaVueloStr = vuelo->vuelo.FechaOrigen; // Suponiendo que la fecha está en el campo FechaOrigen

				// Comparar la fecha del vuelo con las fechas seleccionadas
				if (strcmp(fechaVueloStr, fechaInicioStr) >= 0 && strcmp(fechaVueloStr, fechaFinalStr) <= 0) {
					// La fecha del vuelo está dentro del rango especificado, agregar el vuelo al listbox
					char str[MAX_PATH];
					sprintf_s(str, "%d - %s", vuelo->vuelo.NumVuelo, fechaVueloStr);
					SendMessageA(GetDlgItem(hwnd, VS_Vuelos2), LB_ADDSTRING, 0, (LPARAM)str);
				}
				vuelo = vuelo->siguiente;
			}
		} break;
		case VS_Asiento: {
			// Obtener el handle del ListBox de vuelos
			HWND listBoxVuelosAsiento = GetDlgItem(hwnd, VMAN_Vuelos);
			if (listBoxVuelosAsiento != NULL) {
				// Obtener el índice del elemento seleccionado
				int selectedIndexAsiento = SendMessage(listBoxVuelosAsiento, LB_GETCURSEL, 0, 0);
				if (selectedIndexAsiento != LB_ERR) {
					// Obtener el puntero al nodo del vuelo asociado al índice seleccionado
					NodoVuelo* vueloSeleccionadoAsiento = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelosAsiento, LB_GETITEMDATA, selectedIndexAsiento, 0));
					if (vueloSeleccionadoAsiento != nullptr) {
						// Limpiar el ListBox antes de agregar los elementos
						HWND listboxBoletoAsiento = GetDlgItem(hwnd, VS_Pasajeros);
						if (listboxBoletoAsiento != NULL) {
							SendMessage(listboxBoletoAsiento, LB_RESETCONTENT, 0, 0);

							// Insertar los boletos en el ListBox según el asiento
							NodoBoleto* aletraAsiento = ListaBoletos.cabeza;
							while (aletraAsiento != nullptr) {
								if (aletraAsiento->boleto.numVuelo == vueloSeleccionadoAsiento->vuelo.NumVuelo) {
									char str[300];
									sprintf_s(str, "%c%d - %s %s %s - %s",
										aletraAsiento->boleto.Aletra,
										aletraAsiento->boleto.Anumero,
										aletraAsiento->boleto.NombrePasajero,
										aletraAsiento->boleto.ApellidoP,
										aletraAsiento->boleto.ApellidoM,
										aletraAsiento->boleto.estatus);
									int index = SendMessageA(listboxBoletoAsiento, LB_ADDSTRING, 0, (LPARAM)str);
									SendMessage(listboxBoletoAsiento, LB_SETITEMDATA, index, (LPARAM)aletraAsiento);
								}
								aletraAsiento = aletraAsiento->siguiente;
							}
						}
					}
				}
			}
			break;
		}
		case VS_Apellido: {
			// Obtener el handle del ListBox de vuelos
			HWND listBoxVuelosApellido = GetDlgItem(hwnd, VMAN_Vuelos);
			if (listBoxVuelosApellido != NULL) {
				// Obtener el índice del elemento seleccionado
				int selectedIndexApellido = SendMessage(listBoxVuelosApellido, LB_GETCURSEL, 0, 0);
				if (selectedIndexApellido != LB_ERR) {
					// Obtener el puntero al nodo del vuelo asociado al índice seleccionado
					NodoVuelo* vueloSeleccionadoApellido = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelosApellido, LB_GETITEMDATA, selectedIndexApellido, 0));
					if (vueloSeleccionadoApellido != nullptr) {
						// Limpiar el ListBox antes de agregar los elementos
						HWND listboxBoletoApellido = GetDlgItem(hwnd, VS_Pasajeros);
						if (listboxBoletoApellido != NULL) {
							SendMessage(listboxBoletoApellido, LB_RESETCONTENT, 0, 0);

							// Insertar los boletos en el ListBox según el apellido
							NodoBoleto* aletraApellido = ListaBoletos.cabeza;
							while (aletraApellido != nullptr) {
								if (aletraApellido->boleto.numVuelo == vueloSeleccionadoApellido->vuelo.NumVuelo) {
									char str[300];
									sprintf_s(str, "%s %s %s - %c%d - %s",
										aletraApellido->boleto.ApellidoP,
										aletraApellido->boleto.ApellidoM,
										aletraApellido->boleto.NombrePasajero,
										aletraApellido->boleto.Aletra,
										aletraApellido->boleto.Anumero,
										aletraApellido->boleto.estatus);
									int index = SendMessageA(listboxBoletoApellido, LB_ADDSTRING, 0, (LPARAM)str);
									SendMessage(listboxBoletoApellido, LB_SETITEMDATA, index, (LPARAM)aletraApellido);
								}
								aletraApellido = aletraApellido->siguiente;
							}
						}
					}
				}
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

	}break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(117);
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK Pasajeros(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{

	case WM_INITDIALOG: {

		// Cargar la nueva imagen del usuario actual
		HBITMAP nuevaImagen = (HBITMAP)LoadImageA(NULL, usuarioActual.RutaImagen, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		if (nuevaImagen != NULL) {
			// Establecer la nueva imagen en el control de imagen correspondiente
			SendMessage(GetDlgItem(hwnd, US_ActualImage), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)nuevaImagen);
		}

		// Obtener el identificador del control de edición US_ActualUser
		HWND editControl = GetDlgItem(hwnd, US_ActualUser);

		if (editControl != NULL) {
			// Establecer el texto del control con el nombre de usuario actual
			SetWindowTextA(editControl, usuarioActual.Username);
		}

		// Obtener el handle del ComboBox de género usando el identificador del recurso de diálogo
		HWND genero = GetDlgItem(hwnd, RR_Genero);

		// Agregar elementos al ComboBox
		SendMessage(genero, CB_ADDSTRING, 0, (LPARAM)L"Masculino");
		SendMessage(genero, CB_ADDSTRING, 0, (LPARAM)L"Femenino");

		// Obtener el handle del ComboBox de género usando el identificador del recurso de diálogo
		HWND genero2 = GetDlgItem(hwnd, RM_Genero2);

		// Agregar elementos al ComboBox
		SendMessage(genero2, CB_ADDSTRING, 0, (LPARAM)L"Masculino");
		SendMessage(genero2, CB_ADDSTRING, 0, (LPARAM)L"Femenino");


		// Cargar la lista de pasajeros en el ListBox RP_Pasajeros
		HWND listBoxPasajeros = GetDlgItem(hwnd, RP_Pasajero);
		if (listBoxPasajeros != NULL) {
			// Limpiar el ListBox antes de agregar los elementos
			SendMessage(listBoxPasajeros, LB_RESETCONTENT, 0, 0);

			// Iterar sobre la lista de pasajeros y agregar cada uno al ListBox
			NodoPasajero* pasajero = listaPasajero.cabeza;
			while (pasajero != nullptr) {
				// Crear una cadena con el nombre y los apellidos del pasajero
				char buffer[300];
				snprintf(buffer, sizeof(buffer), "%s %s %s", pasajero->pasajero.Nombre, pasajero->pasajero.Apellido_P, pasajero->pasajero.Apellido_M);

				// Agregar la cadena al ListBox
				int index = SendMessageA(listBoxPasajeros, LB_ADDSTRING, 0, (LPARAM)buffer);
				// Establecer el puntero al nodo del pasajero como el dato asociado al índice
				SendMessage(listBoxPasajeros, LB_SETITEMDATA, index, (LPARAM)pasajero);

				// Avanzar al siguiente pasajero
				pasajero = pasajero->siguiente;
			}
		}

	}break;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		if (menu(wmId, hwnd))
			return FALSE;

		switch (wmId)
		{
		case RP_Pasajero: // Capturar el evento de selección del ListBox
			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxPasajeros = GetDlgItem(hwnd, RP_Pasajero);
				if (listBoxPasajeros != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxPasajeros, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoPasajero* pasajeroSeleccionado = reinterpret_cast<NodoPasajero*>(SendMessage(listBoxPasajeros, LB_GETITEMDATA, selectedIndex, 0));
						if (pasajeroSeleccionado != nullptr) {
							// Establecer la información del pasajero en los campos RP_Nombre, RP_Apaterno y RP_Amaterno
							SetWindowTextA(GetDlgItem(hwnd, RP_Nombre), pasajeroSeleccionado->pasajero.Nombre);
							SetWindowTextA(GetDlgItem(hwnd, RP_Apatern), pasajeroSeleccionado->pasajero.Apellido_P);
							SetWindowTextA(GetDlgItem(hwnd, RP_Amaterno), pasajeroSeleccionado->pasajero.Apellido_M);
							// Establecer la información del pasajero en los campos RP_Nombre, RP_Apaterno y RP_Amaterno
							SetWindowTextA(GetDlgItem(hwnd, RM_Nombre), pasajeroSeleccionado->pasajero.Nombre);
							SetWindowTextA(GetDlgItem(hwnd, RM_ApellidoP), pasajeroSeleccionado->pasajero.Apellido_P);
							SetWindowTextA(GetDlgItem(hwnd, RM_ApellidoM), pasajeroSeleccionado->pasajero.Apellido_M);
							SetWindowTextA(GetDlgItem(hwnd, RM_Nacionalidad), pasajeroSeleccionado->pasajero.Nacionalidad);
							SetWindowTextA(GetDlgItem(hwnd, RM_FechaNacimiento), pasajeroSeleccionado->pasajero.FechaNacimiento);
							SetWindowTextA(GetDlgItem(hwnd, RM_Gen), pasajeroSeleccionado->pasajero.Genero);

						}
					}
				}
			}
			break;
		case RR_Registrar: {
			char nombre[MAX_PATH], aPaterno[MAX_PATH], aMaterno[MAX_PATH], genero[MAX_PATH],
				nacionalidad[MAX_PATH], fechaNacimiento[MAX_PATH];

			GetWindowTextA(GetDlgItem(hwnd, RR_Nombre), nombre, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, RR_ApellidoP), aPaterno, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, RR_ApellidoM), aMaterno, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, RR_Genero), genero, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, RR_Nacionalidad), nacionalidad, MAX_PATH);
			// Obtener la fecha de nacimiento en formato SYSTEMTIME
			SYSTEMTIME Nacimiento;
			SendMessage(GetDlgItem(hwnd, RR_FechaNacimiento), DTM_GETSYSTEMTIME, 0, (LPARAM)&Nacimiento);

			// Convertir la fecha de nacimiento a una cadena de caracteres con formato
			GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &Nacimiento, NULL, fechaNacimiento, MAX_PATH);

			// Verificar que todos los campos estén completos
			if (strlen(nombre) == 0 || strlen(aPaterno) == 0 || strlen(aMaterno) == 0 || strlen(genero) == 0 ||
				strlen(nacionalidad) == 0 || strlen(fechaNacimiento) == 0) {
				// Mostrar un mensaje de advertencia si algún campo está vacío
				MessageBox(hwnd, L"Todos los campos deben completarse.", L"Advertencia", MB_OK | MB_ICONWARNING);
			}
			else {
				// Crear un nuevo pasajero y agregarlo a la lista
				Pasajero nuevoPasajero;
				strcpy_s(nuevoPasajero.Nombre, nombre);
				strcpy_s(nuevoPasajero.Apellido_P, aPaterno);
				strcpy_s(nuevoPasajero.Apellido_M, aMaterno);
				strcpy_s(nuevoPasajero.Genero, genero);
				strcpy_s(nuevoPasajero.Nacionalidad, nacionalidad);
				strcpy_s(nuevoPasajero.FechaNacimiento, fechaNacimiento);

				listaPasajero.agregarAlInicio(nuevoPasajero);
				listaPasajero.guardar(rutaGeneral);

				// Mostrar un mensaje de éxito
				MessageBox(hwnd, L"Registro exitoso.", L"Exito", MB_OK | MB_ICONINFORMATION);
			}

		}break;
		case RP_Aceptar: {

		} break;
		case RP_Cancelar: {

		} break;
		case RM_Eliminar: {
			// Obtener el handle del ListBox RP_Pasajero
			HWND listBoxPasajeros = GetDlgItem(hwnd, RP_Pasajero);
			if (listBoxPasajeros != NULL) {
				// Obtener el índice del elemento seleccionado
				int selectedIndex = SendMessage(listBoxPasajeros, LB_GETCURSEL, 0, 0);
				if (selectedIndex != LB_ERR) {
					// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
					NodoPasajero* pasajeroSeleccionado = reinterpret_cast<NodoPasajero*>(SendMessage(listBoxPasajeros, LB_GETITEMDATA, selectedIndex, 0));
					if (pasajeroSeleccionado != nullptr) {
						// Mostrar un mensaje de confirmación
						int result = MessageBox(hwnd, L"Estas seguro de que quieres eliminar este pasajero?", L"Confirmar eliminacion", MB_YESNO | MB_ICONQUESTION);
						if (result == IDYES) {
							// Eliminar el nodo del pasajero seleccionado
							listaPasajero.eliminarNodo(pasajeroSeleccionado);

							// Guardar los cambios en el archivo
							listaPasajero.guardar(rutaGeneral);

							// Actualizar el ListBox RP_Pasajero para reflejar los cambios
							SendMessage(listBoxPasajeros, LB_RESETCONTENT, 0, 0);
							NodoPasajero* pasajero = listaPasajero.cabeza;
							while (pasajero != nullptr) {
								char buffer[300];
								snprintf(buffer, sizeof(buffer), "%s %s %s", pasajero->pasajero.Nombre, pasajero->pasajero.Apellido_P, pasajero->pasajero.Apellido_M);
								int index = SendMessageA(listBoxPasajeros, LB_ADDSTRING, 0, (LPARAM)buffer);
								SendMessage(listBoxPasajeros, LB_SETITEMDATA, index, (LPARAM)pasajero);
								pasajero = pasajero->siguiente;
							}

							// Limpiar los campos RM_Nombre, RM_Apaterno, RM_Amaterno, RM_Nacionalidad y RM_FechaNacimiento
							SetWindowTextA(GetDlgItem(hwnd, RM_Nombre), "");
							SetWindowTextA(GetDlgItem(hwnd, RM_ApellidoP), "");
							SetWindowTextA(GetDlgItem(hwnd, RM_ApellidoM), "");
							SetWindowTextA(GetDlgItem(hwnd, RM_Nacionalidad), "");
							SetWindowTextA(GetDlgItem(hwnd, RM_FechaNacimiento), "");
							// Limpiar el ComboBox RM_Genero
							SendMessage(GetDlgItem(hwnd, RM_Genero), CB_RESETCONTENT, 0, 0);

							// Mostrar un mensaje de éxito
							MessageBox(hwnd, L"Pasajero eliminado con exito.", L"Exito", MB_OK | MB_ICONINFORMATION);
						}
					}
				}
			}
		}break;


		case RM_Actualizar: {
			// Obtener el handle del ListBox RP_Pasajero
			HWND listBoxPasajeros = GetDlgItem(hwnd, RP_Pasajero);
			if (listBoxPasajeros != NULL) {
				// Obtener el índice del elemento seleccionado
				int selectedIndex = SendMessage(listBoxPasajeros, LB_GETCURSEL, 0, 0);
				if (selectedIndex != LB_ERR) {
					// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
					NodoPasajero* pasajeroSeleccionado = reinterpret_cast<NodoPasajero*>(SendMessage(listBoxPasajeros, LB_GETITEMDATA, selectedIndex, 0));
					if (pasajeroSeleccionado != nullptr) {
						// Obtener los nuevos valores de los campos
						char nuevoNombre[100];
						char nuevoApellidoP[100];
						char nuevoApellidoM[100];
						char nuevaNacionalidad[100];
						char nuevoGenero[100];
						char nuevaFechaNacimiento[100];

						GetWindowTextA(GetDlgItem(hwnd, RM_Nombre), nuevoNombre, sizeof(nuevoNombre));
						GetWindowTextA(GetDlgItem(hwnd, RM_ApellidoP), nuevoApellidoP, sizeof(nuevoApellidoP));
						GetWindowTextA(GetDlgItem(hwnd, RM_ApellidoM), nuevoApellidoM, sizeof(nuevoApellidoM));
						GetWindowTextA(GetDlgItem(hwnd, RM_Nacionalidad), nuevaNacionalidad, sizeof(nuevaNacionalidad));
						GetWindowTextA(GetDlgItem(hwnd, RM_Genero2), nuevoGenero, sizeof(nuevoGenero));

						// Obtener la fecha seleccionada del DateTimePicker RM_FechaNacimiento2
						SYSTEMTIME nuevaFecha;
						SendMessage(GetDlgItem(hwnd, RM_FechaNacimiento2), DTM_GETSYSTEMTIME, 0, (LPARAM)&nuevaFecha);
						GetDateFormatA(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &nuevaFecha, NULL, nuevaFechaNacimiento, sizeof(nuevaFechaNacimiento));

						// Crear un nuevo pasajero con los nuevos valores
						Pasajero nuevoPasajero;
						strcpy_s(nuevoPasajero.Nombre, nuevoNombre);
						strcpy_s(nuevoPasajero.Apellido_P, nuevoApellidoP);
						strcpy_s(nuevoPasajero.Apellido_M, nuevoApellidoM);
						strcpy_s(nuevoPasajero.Nacionalidad, nuevaNacionalidad);
						strcpy_s(nuevoPasajero.Genero, nuevoGenero);
						strcpy_s(nuevoPasajero.FechaNacimiento, nuevaFechaNacimiento);

						// Modificar el nodo seleccionado con los nuevos valores
						listaPasajero.modificarNodo(pasajeroSeleccionado, nuevoPasajero);

						// Guardar los cambios en el archivo
						listaPasajero.guardar(rutaGeneral);

						// Mostrar un mensaje de éxito
						MessageBox(hwnd, L"Datos actualizados con exito.", L"Exito", MB_OK | MB_ICONINFORMATION);
					}
				}
			}
		}break;


		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(117);
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK Boletos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{

	case WM_INITDIALOG: {
		ListaVuelos.ordenarPorQuicksort();
		// Cargar la nueva imagen del usuario actual
		HBITMAP nuevaImagen = (HBITMAP)LoadImageA(NULL, usuarioActual.RutaImagen, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		if (nuevaImagen != NULL) {
			// Establecer la nueva imagen en el control de imagen correspondiente
			SendMessage(GetDlgItem(hwnd, US_ActualImage), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)nuevaImagen);
		}

		// Obtener el identificador del control de edición US_ActualUser
		HWND editControl = GetDlgItem(hwnd, US_ActualUser);

		if (editControl != NULL) {
			// Establecer el texto del control con el nombre de usuario actual
			SetWindowTextA(editControl, usuarioActual.Username);
		}

		// Obtener el handle del ComboBox de género usando el identificador del recurso de diálogo
		HWND TipoBoleto = GetDlgItem(hwnd, BC_TipoBoleto);
		// Agregar elementos al ComboBox
		SendMessage(TipoBoleto, CB_ADDSTRING, 0, (LPARAM)L"Turista");
		SendMessage(TipoBoleto, CB_ADDSTRING, 0, (LPARAM)L"Ejecutivo");

		HWND MetodoPago = GetDlgItem(hwnd, BC_MetodoPago);
		// Agregar elementos al ComboBox
		SendMessage(MetodoPago, CB_ADDSTRING, 0, (LPARAM)L"Tarjeta de debito");
		SendMessage(MetodoPago, CB_ADDSTRING, 0, (LPARAM)L"Tarjeta de credito");
		SendMessage(MetodoPago, CB_ADDSTRING, 0, (LPARAM)L"Efectivo");

		HWND listboxVuelos = GetDlgItem(hwnd, BC_Vuelos);
		if (listboxVuelos != NULL) {
			SendMessage(listboxVuelos, LB_RESETCONTENT, 0, 0);

			NodoVuelo* vuelo = ListaVuelos.cabeza;
			while (vuelo != nullptr) {
				char str[10];
				sprintf_s(str, "%d", vuelo->vuelo.NumVuelo);
				int index = SendMessageA(listboxVuelos, LB_ADDSTRING, 0, (LPARAM)str);
				SendMessage(listboxVuelos, LB_SETITEMDATA, index, (LPARAM)vuelo);
				vuelo = vuelo->siguiente;

			}
		}
		// Cargar la lista de pasajeros en el ListBox RP_Pasajeros
		HWND listBoxPasajeros = GetDlgItem(hwnd, BC_Pasajero);
		if (listBoxPasajeros != NULL) {
			// Limpiar el ListBox antes de agregar los elementos
			SendMessage(listBoxPasajeros, LB_RESETCONTENT, 0, 0);

			// Iterar sobre la lista de pasajeros y agregar cada uno al ListBox
			NodoPasajero* pasajero = listaPasajero.cabeza;
			while (pasajero != nullptr) {
				// Crear una cadena con el nombre y los apellidos del pasajero
				char buffer[300];
				snprintf(buffer, sizeof(buffer), "%s %s %s", pasajero->pasajero.Nombre, pasajero->pasajero.Apellido_P, pasajero->pasajero.Apellido_M);

				// Agregar la cadena al ListBox
				int index = SendMessageA(listBoxPasajeros, LB_ADDSTRING, 0, (LPARAM)buffer);
				// Establecer el puntero al nodo del pasajero como el dato asociado al índice
				SendMessage(listBoxPasajeros, LB_SETITEMDATA, index, (LPARAM)pasajero);

				// Avanzar al siguiente pasajero
				pasajero = pasajero->siguiente;
			}
		}

		// Llenar listboxes de boletos
		HWND listboxTodos = GetDlgItem(hwnd, BCAN_Pasajeros);
		HWND listboxCancelados = GetDlgItem(hwnd, BCAN_Pasajeros2);
		HWND listboxActivos = GetDlgItem(hwnd, BCAN_Pasajeros3);

		if (listboxTodos != NULL && listboxCancelados != NULL && listboxActivos != NULL) {
			SendMessage(listboxTodos, LB_RESETCONTENT, 0, 0);
			SendMessage(listboxCancelados, LB_RESETCONTENT, 0, 0);
			SendMessage(listboxActivos, LB_RESETCONTENT, 0, 0);

			NodoBoleto* boleto = ListaBoletos.cabeza;
			while (boleto != nullptr) {
				char buffer[300];
				snprintf(buffer, sizeof(buffer), "Vuelo %d, Asiento %c%d: %s %s",
					boleto->boleto.numVuelo, boleto->boleto.Aletra, boleto->boleto.Anumero,
					boleto->boleto.NombrePasajero, boleto->boleto.ApellidoP);

				int indexTodos = SendMessageA(listboxTodos, LB_ADDSTRING, 0, (LPARAM)buffer);
				SendMessage(listboxTodos, LB_SETITEMDATA, indexTodos, (LPARAM)boleto);

				if (strcmp(boleto->boleto.estatus, "Cancelado") == 0) {
					int indexCancelados = SendMessageA(listboxCancelados, LB_ADDSTRING, 0, (LPARAM)buffer);
					SendMessage(listboxCancelados, LB_SETITEMDATA, indexCancelados, (LPARAM)boleto);
				}
				else if (strcmp(boleto->boleto.estatus, "Activo") == 0) {
					int indexActivos = SendMessageA(listboxActivos, LB_ADDSTRING, 0, (LPARAM)buffer);
					SendMessage(listboxActivos, LB_SETITEMDATA, indexActivos, (LPARAM)boleto);
				}

				boleto = boleto->siguiente;
			}
		}
	}break;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		if (menu(wmId, hwnd))
			return FALSE;

		switch (wmId)
		{
		case BC_Vuelos2: {

			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxVuelos = GetDlgItem(hwnd, BC_Vuelos2);
				if (listBoxVuelos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoAsiento* AsientoSeleccionado = reinterpret_cast<NodoAsiento*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
						if (AsientoSeleccionado != nullptr) {

							char Numero[10];
							sprintf_s(Numero, "%d", AsientoSeleccionado->asiento.numero);
							SetWindowTextA(GetDlgItem(hwnd, BC_Anumero), Numero);
							char texto[2] = { AsientoSeleccionado->asiento.fila, '\0' };
							SetWindowTextA(GetDlgItem(hwnd, BC_Aletra), texto);

						}
					}
				}
			}
		}break;
		case BCAN_Pasajeros: {
			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxBoletos = GetDlgItem(hwnd, BCAN_Pasajeros);
				if (listBoxBoletos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxBoletos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoBoleto* vueloSeleccionado = reinterpret_cast<NodoBoleto*>(SendMessage(listBoxBoletos, LB_GETITEMDATA, selectedIndex, 0));
						if (vueloSeleccionado != nullptr) {

							char Vuelostr[10];
							sprintf_s(Vuelostr, "%d", vueloSeleccionado->boleto.numVuelo);
							char Totalesstr[10];
							sprintf_s(Totalesstr, "%d", vueloSeleccionado->boleto.costoBoleto);

							SetWindowTextA(GetDlgItem(hwnd, BCAN_Origen), vueloSeleccionado->boleto.origen);
							SetWindowTextA(GetDlgItem(hwnd, BCAN_Destino), vueloSeleccionado->boleto.destino);
							SetWindowTextA(GetDlgItem(hwnd, BCAN_FechaSalida), vueloSeleccionado->boleto.fechaSalida);
							SetWindowTextA(GetDlgItem(hwnd, BCAN_HoraSalida), vueloSeleccionado->boleto.horaSalida);
							SetWindowTextA(GetDlgItem(hwnd, BCAN_TipoAvion), vueloSeleccionado->boleto.tipoBoleto);
							SetWindowTextA(GetDlgItem(hwnd, BCAN_MetodoPago), vueloSeleccionado->boleto.metodoPago);
							SetWindowTextA(GetDlgItem(hwnd, BCAN_NumVuelo), Vuelostr);
							SetWindowTextA(GetDlgItem(hwnd, BCAN_Costo), Totalesstr);


						}
					}
				}
			}
			break;
		}break;
		case BCAN_Cancelar: {
			// Obtener el handle del ListBox
			HWND listBoxBoletos = GetDlgItem(hwnd, BCAN_Pasajeros);
			if (listBoxBoletos != NULL) {
				// Obtener el índice del elemento seleccionado
				int selectedIndex = SendMessage(listBoxBoletos, LB_GETCURSEL, 0, 0);
				if (selectedIndex != LB_ERR) {
					// Obtener el puntero al nodo del boleto asociado al índice seleccionado
					NodoBoleto* boletoSeleccionado = reinterpret_cast<NodoBoleto*>(SendMessage(listBoxBoletos, LB_GETITEMDATA, selectedIndex, 0));
					if (boletoSeleccionado != nullptr) {
						// Verificar si el boleto ya está cancelado
						if (strcmp(boletoSeleccionado->boleto.estatus, "Cancelado") == 0) {
							MessageBoxA(hwnd, "El boleto ya esta cancelado.", "Error", MB_ICONERROR);
							ListaBoletos.guardar(rutaGeneral6);
						}
						else {
							// Cambiar el estatus del boleto a "Cancelado"
							strcpy_s(boletoSeleccionado->boleto.estatus, "Cancelado");
							MessageBoxA(hwnd, "El boleto ha sido cancelado.", "Cancelacion Exitosa", MB_OK | MB_ICONINFORMATION);
						}
					}
				}
			}
			break;
		} break;

		case BC_Vuelos: {
			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxVuelos = GetDlgItem(hwnd, BC_Vuelos);
				if (listBoxVuelos != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxVuelos, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoVuelo* vueloSeleccionado = reinterpret_cast<NodoVuelo*>(SendMessage(listBoxVuelos, LB_GETITEMDATA, selectedIndex, 0));
						if (vueloSeleccionado != nullptr) {
							char Vuelostr[10];
							sprintf_s(Vuelostr, "%d", vueloSeleccionado->vuelo.NumVuelo);
							char Totalesstr[10];
							sprintf_s(Totalesstr, "%d", vueloSeleccionado->vuelo.NumAsientosTot);
							char Ejecutivosstr[10];
							sprintf_s(Ejecutivosstr, "%d", vueloSeleccionado->vuelo.NumAsientoEj);
							char Turistasstr[10];
							sprintf_s(Turistasstr, "%d", vueloSeleccionado->vuelo.NumAsientoTu);
							char Pmenoresstr[10];
							sprintf_s(Pmenoresstr, "%d", vueloSeleccionado->vuelo.Menores);
							char Pmayoresstr[10];
							sprintf_s(Pmayoresstr, "%d", vueloSeleccionado->vuelo.Mayores);
							char Amenoresstr[10];
							sprintf_s(Amenoresstr, "%d", vueloSeleccionado->vuelo.asientosMenores);
							char Amayoresstr[10];
							sprintf_s(Amayoresstr, "%d", vueloSeleccionado->vuelo.asientosMayores);
							char Cejestr[10];
							sprintf_s(Cejestr, "%d", vueloSeleccionado->vuelo.CostoEj);
							char Cturstr[10];
							sprintf_s(Cturstr, "%d", vueloSeleccionado->vuelo.CostoTu);

							SetWindowTextA(GetDlgItem(hwnd, BC_Origen), vueloSeleccionado->vuelo.Origen);
							SetWindowTextA(GetDlgItem(hwnd, BC_Destino), vueloSeleccionado->vuelo.Destino);
							SetWindowTextA(GetDlgItem(hwnd, BC_FechaOrigen), vueloSeleccionado->vuelo.FechaOrigen);
							SetWindowTextA(GetDlgItem(hwnd, BC_HoraOrigen), vueloSeleccionado->vuelo.HoraOrigen);

							SetWindowTextA(GetDlgItem(hwnd, BC_NumVuelo), Vuelostr);

							SetWindowTextA(GetDlgItem(hwnd, BC_NumAsientoTot), Totalesstr);
							SetWindowTextA(GetDlgItem(hwnd, BC_NumAsientoEj), Ejecutivosstr);
							SetWindowTextA(GetDlgItem(hwnd, BC_NumAsientoTu), Turistasstr);

							SetWindowTextA(GetDlgItem(hwnd, BC_Menores), Amenoresstr);
							SetWindowTextA(GetDlgItem(hwnd, BC_Mayores), Amayoresstr);

							SetWindowTextA(GetDlgItem(hwnd, BC_Costo), Cejestr);
							SetWindowTextA(GetDlgItem(hwnd, BC_Costo2), Cturstr);

							HWND listboxLetra = GetDlgItem(hwnd, BC_Vuelos2);
							if (listboxLetra != NULL) {
								SendMessage(listboxLetra, LB_RESETCONTENT, 0, 0);

								NodoAsiento* Aletra = ListaAsientos.cabeza;
								while (Aletra != nullptr) {
									if (Aletra->asiento.numAvion == vueloSeleccionado->vuelo.NumVuelo) {
										if (Aletra->asiento.disponible) { // Solo agrega los asientos disponibles
											char str[10];
											sprintf_s(str, "%c%d", Aletra->asiento.fila, Aletra->asiento.numero);
											int index = SendMessageA(listboxLetra, LB_ADDSTRING, 0, (LPARAM)str);
											SendMessage(listboxLetra, LB_SETITEMDATA, index, (LPARAM)Aletra);
										}
									}
									Aletra = Aletra->siguiente;
								}
							}
						}
					}
				}
			}
		}break;
		case BC_Pasajero: {
			if (wmEvent == LBN_SELCHANGE) {
				// Obtener el handle del ListBox
				HWND listBoxPasajeros = GetDlgItem(hwnd, BC_Pasajero);
				if (listBoxPasajeros != NULL) {
					// Obtener el índice del elemento seleccionado
					int selectedIndex = SendMessage(listBoxPasajeros, LB_GETCURSEL, 0, 0);
					if (selectedIndex != LB_ERR) {
						// Obtener el puntero al nodo del pasajero asociado al índice seleccionado
						NodoPasajero* pasajeroSeleccionado = reinterpret_cast<NodoPasajero*>(SendMessage(listBoxPasajeros, LB_GETITEMDATA, selectedIndex, 0));
						if (pasajeroSeleccionado != nullptr) {
							// Establecer la información del pasajero en los campos RP_Nombre, RP_Apaterno y RP_Amaterno
							SetWindowTextA(GetDlgItem(hwnd, BC_Nombre), pasajeroSeleccionado->pasajero.Nombre);
							SetWindowTextA(GetDlgItem(hwnd, BC_ApellidoM), pasajeroSeleccionado->pasajero.Apellido_P);
							SetWindowTextA(GetDlgItem(hwnd, BC_ApellidoP), pasajeroSeleccionado->pasajero.Apellido_M);


						}
					}
				}
			}
		}break;
		case BC_Comprar: {
			char NombrePasajero[MAX_PATH], origen[MAX_PATH], destino[MAX_PATH], fechaSalida[MAX_PATH], horaSalida[MAX_PATH], tipoBoleto[MAX_PATH], boletoGenerado[MAX_PATH],
				metodoPago[MAX_PATH], estatus[MAX_PATH], Apaterno[MAX_PATH], Amaterno[MAX_PATH];
			char numVuelostr[MAX_PATH], costoBoletostr[MAX_PATH], costoBoletostr2[MAX_PATH], Anumerostr[MAX_PATH], Aletra[MAX_PATH];
			int numVuelo, costoBoleto, Anumero, costoTurista, costoEjecutivo;
			bool banderaboleto;

			GetWindowTextA(GetDlgItem(hwnd, BC_Nombre), NombrePasajero, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, BC_ApellidoP), Apaterno, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, BC_ApellidoM), Amaterno, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, BC_Origen), origen, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, BC_Destino), destino, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, BC_FechaOrigen), fechaSalida, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, BC_HoraOrigen), horaSalida, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, BC_TipoBoleto), tipoBoleto, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, BC_MetodoPago), metodoPago, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, BC_NumVuelo), numVuelostr, MAX_PATH);
			GetWindowTextA(GetDlgItem(hwnd, BC_Costo), costoBoletostr, MAX_PATH);//turista
			GetWindowTextA(GetDlgItem(hwnd, BC_Costo2), costoBoletostr2, MAX_PATH);//ejecutivo
			GetWindowTextA(GetDlgItem(hwnd, BC_Aletra), Aletra, MAX_PATH);//turista
			GetWindowTextA(GetDlgItem(hwnd, BC_Anumero), Anumerostr, MAX_PATH);//ejecutivo; pedos aquí revisar bien 

			numVuelo = atoi(numVuelostr);
			Anumero = atoi(Anumerostr);
			costoTurista = atoi(costoBoletostr);
			costoEjecutivo = atoi(costoBoletostr2);


			Boleto nuevoBoleto;

			strcpy_s(nuevoBoleto.NombrePasajero, NombrePasajero);
			strcpy_s(nuevoBoleto.ApellidoP, Apaterno);
			strcpy_s(nuevoBoleto.ApellidoM, Amaterno);
			strcpy_s(nuevoBoleto.origen, origen);
			strcpy_s(nuevoBoleto.destino, destino);
			strcpy_s(nuevoBoleto.fechaSalida, fechaSalida);
			strcpy_s(nuevoBoleto.horaSalida, horaSalida);
			strcpy_s(nuevoBoleto.tipoBoleto, tipoBoleto);
			strcpy_s(nuevoBoleto.metodoPago, metodoPago);
			nuevoBoleto.numVuelo = numVuelo;
			nuevoBoleto.Anumero = Anumero;

			if (nuevoBoleto.tipoBoleto == "Turista") {
				nuevoBoleto.costoBoleto = costoTurista;
			}
			else {
				nuevoBoleto.costoBoleto = costoEjecutivo;
			}
			if (strlen(Aletra) == 1 && Aletra[0] >= 'A' && Aletra[0] <= 'Z') {
				nuevoBoleto.Aletra = Aletra[0]; // Asignar directamente el carácter
			}
			else {
				// Maneja el error: el valor de AletraBuffer no es válido
				MessageBoxA(hwnd, "Por favor, ingresa una letra valida de la A a la Z.", "Error", MB_OK | MB_ICONERROR);
				break;
			}

			ListaBoletos.ordenarBoletos();

			if (!ListaBoletos.verificarBoletoExistente(nuevoBoleto.numVuelo, nuevoBoleto.NombrePasajero, nuevoBoleto.ApellidoP, nuevoBoleto.ApellidoM)) {
				ListaBoletos.agregar(nuevoBoleto);
				ListaBoletos.guardar(rutaGeneral6);
				ListaAsientos.ordenarAsientosPorNumeroVuelo();
				ListaAsientos.ocultar_asiento(nuevoBoleto.Aletra, nuevoBoleto.Anumero, nuevoBoleto.numVuelo);
				ListaAsientos.actualizar_listbox(hwnd, BC_Vuelos2, nuevoBoleto.numVuelo);
				ListaAsientos.guardar(rutaGeneral4);
				MessageBoxA(hwnd, "Se ha guardado el boleto con exito", "EXITO", MB_OK | MB_HELP);
			}
			else {
				MessageBoxA(hwnd, "Este pasajero ha comprado un boleto en este vuelo con anterioridad", "Error", MB_OK | MB_ICONERROR);
			}

		}break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(117);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}break;

	}

	return FALSE;
}

bool menu(int opcion, HWND hwnd) {
	switch (opcion) {
		//Principal--------------------------------------------------------------------------------------------------------
	case ID_INICIO: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_Principal),
			NULL,
			Principal);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	} break;
		//Usuarios---------------------------------------------------------------------------------------------------------
	case ID_USUARIOS_AGREGARUSUARIO: {
		if (strcmp(usuarioActual.Username, adminPredeterminado.usuarioAdmin) == 0) {
			HWND ventana = CreateDialog(Ginst,
				MAKEINTRESOURCE(DLG_AgregarUsuario),
				NULL,
				Usuarios);
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);
		}
		else {
			MessageBox(hwnd, L"Solo el administrador puede acceder.", L"Advertencia", MB_OK | MB_ICONINFORMATION);
		}
	} break;
	case ID_USUARIOS_MODIFICARUSUARIO40018: {
		if (strcmp(usuarioActual.Username, adminPredeterminado.usuarioAdmin) == 0) {
			HWND ventana = CreateDialog(Ginst,
				MAKEINTRESOURCE(DLG_ModificarUsuario),
				NULL,
				Usuarios);
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);
		}
		else {
			MessageBox(hwnd, L"Solo el administrador puede acceder.", L"Advertencia", MB_OK | MB_ICONINFORMATION);
		}
	} break;
	case ID_USUARIOS_ELIMINARUSUARIO: {
		if (strcmp(usuarioActual.Username, adminPredeterminado.usuarioAdmin) == 0) {
			HWND ventana = CreateDialog(Ginst,
				MAKEINTRESOURCE(DLG_EliminarUsuario),
				NULL,
				Usuarios);
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);
		}
		else {
			MessageBox(hwnd, L"Solo el administrador puede acceder.", L"Advertencia", MB_OK | MB_ICONINFORMATION);
		}
	} break;
		//Vuelos-----------------------------------------------------------------------------------------------------------
	case ID_VUELOS_AGREGAR: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_RegistroVuelos),
			NULL,
			Vuelos);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
	case ID_VUELOS_ELIMINAR: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_EliminarVuelo),
			NULL,
			Vuelos);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
	case ID_VUELOS_MODIFICAR: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_ModificarVuelo),
			NULL,
			Vuelos);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
	case ID_VUELOS_CONFIRMACION: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_ConfirmarVuelo),
			NULL,
			Vuelos);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
	case ID_VUELOS_MANIFIESTO: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_ManifiestoVuelo),
			NULL,
			Vuelos);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
	case ID_VUELOS_REPORTE: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_ReporteVuelo),
			NULL,
			Vuelos);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
		//Boletos----------------------------------------------------------------------------------------------------------
	case ID_BOLETOS_COMPRAR: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_CompraBoletos),
			NULL,
			Boletos);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
	case ID_BOLETOS_CANCELAR: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_CancelarBoletos),
			NULL,
			Boletos);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
		//Pasajeros--------------------------------------------------------------------------------------------------------
	case ID_PASAJEROS_REGISTRAR: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_RegistroPasajeros),
			NULL,
			Pasajeros);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
	case ID_PASAJEROS_PASEDEABORDAR: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_PaseAbordar),
			NULL,
			Pasajeros);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
	case ID_PASAJEROS_MODIFICARPASAJERO: {
		HWND ventana = CreateDialog(Ginst,
			MAKEINTRESOURCE(DLG_ModificarPasajero),
			NULL,
			Pasajeros);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);
	}break;
		//Salir------------------------------------------------------------------------------------------------------------
	case  ID_SALIR: {
		PostQuitMessage(117);
		break;
	}break;
	default:
		return false;
	}
	return true;
}
