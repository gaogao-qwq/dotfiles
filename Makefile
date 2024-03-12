DOTFILES := dwm emacs i3 kitty nvim scripts st tmux vim zsh

ALL: reinstall

install:
	stow -S $(DOTFILES)
	@echo "Dotfiles successfully linked to:"
	@sudo find $(HOME) -maxdepth 3 -type l -ls | grep dotfiles

reinstall:
	stow -R $(DOTFILES) 2> /dev/null
	@echo "Dotfiles successfully linked to:"
	@sudo find $(HOME) -maxdepth 3 -type l -ls | grep dotfiles

uninstall:
	stow -D $(DOTFILES) 2> /dev/null
