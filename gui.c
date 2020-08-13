/*
 * GUI frontend for core.c
 *
 * PART OF: echo3d
 * FILENAME: gui.c
 * AUTHOR: Philipp Lensch
 */

#define TITLE "echo3d v0.1" // (version)

#include <stdlib.h>
#include <stdio.h>
#include <iup.h>

#include "core.h"

// GLOBAL
Ihandle *inpath, *pathpath, *outpath; // filepaths
Ihandle *tg_colors, *tg_center, *tg_normals; // toggles / options
Ihandle *lb_status; // labels / status

int cb_normals(void) {
    IupSetAttribute(pathpath, "ACTIVE", IupGetInt(tg_normals, "VALUE") ? "YES" : "NO");

    return IUP_DEFAULT;
}

int cb_run(void)
{
    int status = 1;

    char* in = IupGetAttribute(inpath, "VALUE");
    char* path = IupGetAttribute(pathpath, "VALUE");
    char* out = IupGetAttribute(outpath, "VALUE");
    unsigned int colors = IupGetInt(tg_colors, "VALUE");
    unsigned int center = IupGetInt(tg_center, "VALUE");
    unsigned int normals = IupGetInt(tg_normals, "VALUE");

    if ((in[0] != '\0') && (out[0] != '\0') && ((normals ? (path[0] != '\0') : 1)))
    {
        status = generateNormals(in, path, out, colors, center, normals);
    }

    IupSetAttribute(lb_status, "TITLE", (status) ? "Fail" : "Success");

    return IUP_DEFAULT;
}

int cb_open(void)
{
    Ihandle *filedlg = IupFileDlg();
    IupSetAttributes(filedlg, "DIALOGTYPE=OPEN");
    IupSetAttribute(filedlg, "FILE", IupGetAttribute(inpath, "VALUE"));
    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(filedlg, "STATUS") != -1)
    {
    char* filename = IupGetAttribute(filedlg, "VALUE");
    IupSetStrAttribute(inpath, "VALUE", filename);
    }

  IupDestroy(filedlg);
  return IUP_DEFAULT;
}

int cb_open_path(void)
{
    Ihandle *filedlg = IupFileDlg();
    IupSetAttributes(filedlg, "DIALOGTYPE=OPEN");
    IupSetAttribute(filedlg, "FILE", IupGetAttribute(pathpath, "VALUE"));
    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(filedlg, "STATUS") != -1)
    {
    char* filename = IupGetAttribute(filedlg, "VALUE");
    IupSetStrAttribute(pathpath, "VALUE", filename);
    }

  IupDestroy(filedlg);
  return IUP_DEFAULT;
}

int cb_save(void)
{
    Ihandle *filedlg = IupFileDlg();
    IupSetAttributes(filedlg, "DIALOGTYPE=SAVE");
    IupSetAttribute(filedlg, "FILE", IupGetAttribute(outpath, "VALUE"));
    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(filedlg, "STATUS") != -1)
    {
    char* filename = IupGetAttribute(filedlg, "VALUE");
    IupSetStrAttribute(outpath, "VALUE", filename);
    }

  IupDestroy(filedlg);
  return IUP_DEFAULT;
}

int main(int argc, char **argv)
{
    Ihandle *inlabel, *pathlabel, *outlabel; // LABELS
    Ihandle *dlg, *vbox;
    Ihandle *btn_run, *btn_open, *btn_path, *btn_save; // BUTTONS

    IupOpen(&argc, &argv);
    IupImageLibOpen();

    // MAIN DIALOG
    inlabel = IupLabel("Input PC:");
    inpath = IupText(NULL);
    btn_open = IupButton(NULL, NULL);
    pathlabel = IupLabel("Echosounder Path:");
    pathpath = IupText(NULL);
    btn_path = IupButton(NULL, NULL);
    outlabel = IupLabel("Output PC:");
    outpath = IupText(NULL);
    btn_save = IupButton(NULL, NULL);
    tg_colors = IupToggle("Color by Z", NULL);
    tg_center = IupToggle("Center", NULL);
    tg_normals = IupToggle("Generate Normals", NULL);
    lb_status = IupLabel(NULL);
    btn_run = IupButton("Run", NULL);

    vbox = IupVbox(inlabel,
                   IupHbox(inpath,btn_open), 
                   pathlabel,
                   IupHbox(pathpath, btn_path),
                   outlabel,
                   IupHbox(outpath, btn_save),
                   IupHbox(tg_colors,tg_center,tg_normals),
                   IupHbox(IupLabel("Status:"), lb_status),
                   btn_run,
                   NULL);

    dlg = IupDialog(vbox);
    IupSetAttribute(dlg, "TITLE", TITLE);

    // ATTRIBUTES
    IupSetAttributes(inpath, "MULTILINE=NO, EXPAND=HORIZONTAL");
    IupSetAttributes(pathpath, "MULTILINE=NO, EXPAND=HORIZONTAL, ACTIVE=NO");
    IupSetAttributes(outpath, "MULTILINE=NO, EXPAND=HORIZONTAL");

    IupSetAttributes(btn_open, "IMAGE=IUP_FileOpen");
    IupSetAttributes(btn_path, "IMAGE=IUP_FileOpen");
    IupSetAttributes(btn_save, "IMAGE=IUP_FileSave");
    IupSetAttributes(btn_run, "ALIGNMENT=ARIGHT");

    IupSetCallback(btn_run, "ACTION", (Icallback) cb_run);
    IupSetCallback(btn_open, "ACTION", (Icallback) cb_open);
    IupSetCallback(btn_path, "ACTION", (Icallback) cb_open_path);
    IupSetCallback(btn_save, "ACTION", (Icallback) cb_save);

    IupSetCallback(tg_normals, "ACTION", (Icallback) cb_normals);

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();
  
    IupClose();
    return EXIT_SUCCESS;
}
