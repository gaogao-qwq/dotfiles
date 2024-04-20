;; 自动保存
(auto-save-mode)
;; 退出 emacs 保存 buffer 状态
(desktop-save-mode 1)
;; 关闭响铃
(setq visible-bell nil)
(global-display-line-numbers-mode 1)
(setq display-line-numbers-type 'relative)
;; 不显示菜单栏
(menu-bar-mode -1)
;; 不显示滚动条
(scroll-bar-mode -1)
;; 不显示工具栏
(tool-bar-mode -1)
;; 设置制表符缩进长度
(setq indent-tabs-mode 1)
(setq tab-stop-list (number-sequence 4 120 4))
;; 设置颜色主题
(custom-set-variables
 '(custom-enabled-themes '(leuven-dark))
 '(frame-background-mode 'light))
;; 设置字体
(add-to-list 'default-frame-alist
             '(font . "Maple Mono-12"))

(global-set-key (kbd "C-c l") #'org-store-link)
(global-set-key (kbd "C-c a") #'org-agenda)
(global-set-key (kbd "C-c c") #'org-capture)
