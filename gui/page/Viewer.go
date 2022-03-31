package page

import (
	"errors"
	"fmt"
	"image"
	"io/fs"
	"io/ioutil"
	"os"
	"path/filepath"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/layout"
)

type Page struct {
	filePath string
	img      *canvas.Image
	stat     fs.FileInfo
	config   image.Config
}

type Viewer struct {
	pages           []*Page
	viewContainer   *fyne.Container
	scrollContainer fyne.CanvasObject
}

var supported_ext [3]string = [...]string{".jpg", ".jpeg", ".png"}

func (v *Viewer) isValidExtension(extension string) bool {
	for _, ext := range supported_ext {
		if extension == ext {
			return true
		}
	}
	return false
}

func (v *Viewer) createPage(fileName string, dirPath string) *Page {
	var page *Page = new(Page)
	var err error

	extension := filepath.Ext(fileName)
	if !v.isValidExtension(extension) {
		return nil
	}
	page.filePath = filepath.Join(dirPath, fileName)
	page.stat, err = os.Stat(page.filePath)
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s: %v\n", fileName, err)
		return nil
	}
	if reader, err := os.Open(filepath.Join(dirPath, fileName)); err == nil {
		defer reader.Close()
		config, _, err := image.DecodeConfig(reader)
		if err != nil {
			fmt.Fprintf(os.Stderr, "%s: %v\n", fileName, err)
			return nil
		}
		page.config = config
	}
	return page
}

func (v *Viewer) loadFolderContent(dirPath string) ([]*Page, error) {
	var pageList []*Page

	files, err := ioutil.ReadDir(dirPath)
	if err != nil {
		return nil, errors.New("Fail to load directory content. " + err.Error())
	}
	for _, file := range files {
		if !file.IsDir() {
			page := v.createPage(file.Name(), dirPath)
			if page == nil {
				continue
			}
			pageList = append(pageList, page)
		}
	}
	return pageList, nil
}

func (v *Viewer) zoomOut(scale float32) {
	for _, page := range v.pages {
		width, height := page.config.Width, page.config.Height
		newWidth := float32(width) * scale
		newHeight := float32(height) * scale
		page.img.Resize(fyne.NewSize(newWidth, newHeight))
	}
}

func (v *Viewer) reloadImageView() {
	v.viewContainer.Objects = []fyne.CanvasObject{} // empty
	v.viewContainer.Refresh()
	for _, page := range v.pages {
		if page.img != nil {
			container := container.NewCenter(page.img)
			v.viewContainer.Add(container)
		}
	}
}

func (v *Viewer) LoadFolder(dir_path string) error {
	pageList, err := v.loadFolderContent(dir_path)
	if err != nil {
		return err
	}
	for _, page := range pageList {
		fmt.Println("Load image " + page.filePath)
		page.img = canvas.NewImageFromFile(page.filePath)
		page.img.FillMode = canvas.ImageFillOriginal
		v.pages = append(v.pages, page)
	}
	v.zoomOut(0.5) // DEBUG
	v.reloadImageView()
	return nil
}

func (v *Viewer) LoadUI() fyne.CanvasObject {
	// v.viewContainer = widget.NewList(
	// 	func() int {
	// 		return len(v.pages)
	// 	},
	// 	func() fyne.CanvasObject {
	// 		return canvas.NewImageFromImage()
	// 	},
	// 	func(i widget.ListItemID, o fyne.CanvasObject) {
	// 		o.(*widget.Label).SetText(data[i])
	// 	})
	v.viewContainer = container.NewVBox()

	v.scrollContainer = container.NewVScroll(v.viewContainer)
	centered := container.New(layout.NewHBoxLayout(), layout.NewSpacer(), v.scrollContainer, layout.NewSpacer())
	layout := container.NewBorder(nil, nil, nil, nil, centered)

	return layout
}
