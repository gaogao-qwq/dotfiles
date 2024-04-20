DOTFILES := dwm emacs i3 i3status kitty nvim scripts st tmux vim zsh picom

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

