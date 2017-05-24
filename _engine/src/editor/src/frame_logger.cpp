#include "editor/frame/frame_logger.hpp"

editor::frame_logger::frame_logger(std::shared_ptr<engine::logger_t> logger, wxWindow* parent) : wxFrame(parent, wxID_ANY, _("logger test"), wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE)
{

}

editor::frame_logger::~frame_logger()
{

}

wxBEGIN_EVENT_TABLE(editor::frame_logger, wxFrame)

wxEND_EVENT_TABLE()
