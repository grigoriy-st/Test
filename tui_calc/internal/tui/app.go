package tui

// These imports will be used later in the tutorial. If you save the file
// now, Go might complain they are unused, but that's fine.
// You may also need to run `go mod tidy` to download bubbletea and its
// dependencies.
import (
	"fmt"
	// "os"

	tea "github.com/charmbracelet/bubbletea"
)

func Run() error {
	m := initialModel()
	p := tea.NewProgram(m)

	if err := p.Start(); err != nil {
		return fmt.Errorf("failed to start program: :%w", err)
	}

	return nil
}


