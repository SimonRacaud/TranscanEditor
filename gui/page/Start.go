package page

import "fyne.io/fyne/v2"

type Start struct {
	window fyne.Window
}

func (s *Start) Init(app fyne.App) {
	s.window = app.NewWindow("Title")
	//s.window.SetContent()
	s.window.Resize(fyne.NewSize(800, 600))
	s.window.Show()
}

func NewStartPage() *Start {
	return &Start{}
}
