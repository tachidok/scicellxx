;;; If you want to avoid using  configuration then pass emacs the
;;; "-q" argument

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Start-up
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; Add the package source list
(message "Starting up ...")
(require 'package)
(setq package-enable-at-startup nil)
(add-to-list 'package-archives '("melpa" . "https://melpa.org/packages/"))
;(add-to-list 'package-archives '("melpa-stable" . "https://stable.melpa.org/packages/"))
;;; Initialise packages
(package-initialize)
;;; Refresh the content of the package at startup
(unless (package-installed-p 'use-package)
  (package-refresh-contents)
  (package-install 'use-package))
(message "Starting up ... [DONE]")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Some nice packages
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; If you ever require to disable a package because it is given
;;; problems or you do not require it at the moment then use the
;;; modifier :disabled for the required package, see example in
;;; which-key package

;;; Enable the package which key, very useful when you forget some
;;; emacs commands, just press the initial configuration and wait,
;;; then a list of possibilities will open. Try with C-x and wait.
(use-package which-key
  ;;;:disabled
  :ensure t
  :config
  (which-key-mode)
  (message "which-key ... [DONE]"))

;;; Move through windows using C-x o and then hit the "window number"
;;; you want to move on
(use-package ace-window
  :ensure t
  :init
  (progn
    (global-set-key [remap other-window] 'ace-window)
    (custom-set-faces
     '(aw-leading-char-face
       ((t (:inherit ace-jump-face-foreground :height 3.0))))) 
    )
  :config
  (message "ace-window ... [DONE]"))

;;; Also activate the default key bindings to move through windows by
;;; holding the SHIFT key and then using the arrows
(windmove-default-keybindings)

;;; Use org-mode bullets package
(use-package org-bullets
  :ensure t
  :config
  (add-hook 'org-mode-hook (lambda () (org-bullets-mode 1)))
  (message "org-bullets ... [DONE]"))

(setq org-latex-pdf-process
      '("pdflatex -interaction nonstopmode -output-directory %o %f"
	"bibtex %b"
	"pdflatex -interaction nonstopmode -output-directory %o %f"
	"pdflatex -interaction nonstopmode -output-directory %o %f"))

;;;(setq org-latex-pdf-process (list "latexmk -pdf %f"))

;;;(setq org-latex-pdf-process (quote ("texi2dvi --pdf --clean --verbose
;;;--batch %f" "bibtex %b" "texi2dvi --pdf --clean --verbose --batch %f"
;;;"texi2dvi --pdf --clean --verbose --batch %f")))

;;;(require 'org)
;;;(require 'ox-bibtex)

;;; Adds the ability to redo in emacs, hit C-/ or C-_ to undo but hit
;;; C-shift-/ to redo. You can bring an undo tree to navigate through
;;; the undos by hitting C-x u
(use-package undo-tree
  :ensure t
  :init
  (global-undo-tree-mode)
  :config
  (message "undo-tree ... [DONE]"))

;;; Gives access to the kill ring and allows to use the arrows to
;;; select what we want to paste/yank
(use-package counsel
  :ensure t
  :bind
  (("M-y" . counsel-yank-pop)
   :map ivy-minibuffer-map
   ("M-y" . ivy-next-line))
  :config
  (message "counsel ... [DONE]"))

;;;(global-set-key (kbd "C-x g") 'magit-status)
;;; Magit, for git integration with emacs
(use-package magit
  :ensure t
  :bind
  ("C-x g" . magit-status)
  :config
  (message "magit ... [DONE]"))

;;; Git gutter - Shows modifications or changes from current version
;;; to previous version on git repositories
(use-package git-gutter
  :ensure t
  :init
  (global-git-gutter-mode +1)
  :config
  (message "git-gutter ... [DONE]"))

(use-package git-timemachine
  :ensure t
  :config
  (message "git-timemachine ... [DONE]")
  )

;;;
(use-package julia-mode
  :ensure t
  :config
  (message "julia-mode ... [DONE]")
  )

;;; Latex (using auctex)
(use-package tex
  :ensure auctex
  :config
  (message "tex-auctex ... [DONE]"))

;;; Ivy/swiper/counsel (replace helm)
(use-package swiper
  :ensure t
  :config
  (ivy-mode 1)
  (setq ivy-use-virtual-buffers t)
  (setq ivy-count-format "(%d/%d) ")
  (global-set-key (kbd "C-s") 'swiper)
  (global-set-key (kbd "M-x") 'counsel-M-x)
  (global-set-key (kbd "C-x C-f") 'counsel-find-file)
  (global-set-key (kbd "<f1> f") 'counsel-describe-function)
  (global-set-key (kbd "<f1> v") 'counsel-describe-variable)
  (global-set-key (kbd "<f1> l") 'counsel-find-library)
  (global-set-key (kbd "<f2> i") 'counsel-info-lookup-symbol)
  (global-set-key (kbd "<f2> u") 'counsel-unicode-char)
  (global-set-key (kbd "C-c g") 'counsel-git)
  (global-set-key (kbd "C-c j") 'counsel-git-grep)
  (global-set-key (kbd "C-c k") 'counsel-ag)
  (global-set-key (kbd "C-x l") 'counsel-locate)
  (global-set-key (kbd "C-S-o") 'counsel-rhythmbox)
  (global-set-key (kbd "C-c C-r") 'ivy-resume)
  (message "swiper ... [DONE]"))

;;; projectile (allows to work with projects/repositories)
(use-package projectile
  :ensure t
  :config
  (projectile-mode)
  (setq projectile-completion-system 'ivy)
  (message "projectile ... [DONE]"))

(define-key projectile-mode-map (kbd "C-c p") 'projectile-command-map)

;;; Integration for counsel and projectile
(use-package counsel-projectile
  :ensure t
  :config
                                        ;(counsel-projectile-on) Breaking changes in version 0.2 (no longer exists)
  (counsel-projectile-mode)
  (message "counsel-projectile ... [DONE]"))

;;; ibuffer
(message "ibuffer ...")
(global-set-key (kbd "C-x C-b") 'ibuffer)
(setq ibuffer-saved-filter-groups
      (quote (("default"
	       ("dired" (mode . dired-mode))
	       ("org" (name . "^.*org$"))
               ("mark-down" (name . "^.*md$"))
               ("gnuplot" (or
                           (name . "^.*gp$")
                           (name . "^.*gnuplot$")))
               ("data" (or
                        (name . "^.*dat$")
                        (name . "^.*data$")))
               
               ("web" (or (mode . web-mode) (mode . js2-mode)))
	       ("shell" (or (mode . eshell-mode) (mode . shell-mode)))
	       ("mu4e" (name . "\*mu4e\*"))
	       ("programming" (or
			       (mode . python-mode)
			       (mode . c++-mode)
                               (mode . c-mode)))
	       ("emacs" (or
			 (name . "^\\*scratch\\*$")
			 (name . "^\\*Messages\\*$")
                         (name . "^\\*Warnings\\*$")
                         (name . "^.*el$")))
	       ))))
(add-hook 'ibuffer-mode-hook
	  (lambda ()
	    (ibuffer-auto-mode 1)
	    (ibuffer-switch-to-saved-filter-groups "default")))

;; don't show these
					;(add-to-list 'ibuffer-never-show-predicates "zowie")
;; Don't show filter groups if there are no buffers in that group
(setq ibuffer-show-empty-filter-groups nil)

;; Don't ask for confirmation to delete marked buffers
;;;(setq ibuffer-expert t)
(message "ibuffer ... [DONE]")

;;; gnuplot-mode
(use-package gnuplot-mode
  :ensure t
  :config (setq auto-mode-alist 
                (append '(("\\.\\(gp\\|gnuplot\\)$" . gnuplot-mode)) auto-mode-alist))
  (message "gnuplot-mode ... [DONE]"))

;;; markdowm-mode
(use-package markdown-mode
  :disabled
  :ensure t 
  :commands (markdown-mode gfm-mode)
  :mode (("README\\.md\\'" . gfm-mode)
         ("\\.md\\'" . markdown-mode)
         ("\\.markdown\\'" . markdown-mode))
  :init (setq markdown-command "multimarkdown")
  (message "markdown-mode ... [DONE]"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Some other nice packages (for C/C++ development)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; gg-tags for autocomplete, finding references in files and dumb-jump
(use-package ggtags
;  :disabled ; default [enabled]
  :ensure t
  :config
  (add-hook 'c-mode-common-hook
            (lambda ()
              (when (derived-mode-p 'c-mode 'c++-mode 'java-mode 'asm-mode)
                (ggtags-mode 1))))
  (message "ggtags ... [DONE]"))

;;; Dumb-jump, o jump to function definitions
(use-package dumb-jump
;  :disabled ; default [enabled]
  :ensure t
  :bind (("M-g o" . dumb-jump-go-other-window)
	 ("M-g j" . dumb-jump-go)
	 ("M-g x" . dumb-jump-go-prefer-external)
	 ("M-g z" . dumb-jump-go-prefer-external-other-window))
  :config
  (setq dumb-jump-selector 'ivy) ;; (setq dumb-jump-selector 'helm)
  (message "dumb-jump ... [DONE]"))

(use-package company-irony
;  :disabled ; default [enabled]
  :ensure t
  :config
  (message "company-irony ... [DONE]"))

(use-package company-c-headers
;  :disabled ; default [enabled]
  :ensure t
  :config
  (message "company-c-headers ... [DONE]"))

;;; Company mode (complete anything mode)
(use-package company
;  :disabled ; default [enabled]
  :ensure t
  :config
  (add-hook 'after-init-hook 'global-company-mode)
  (setq company-backends (delete 'company-semantic company-backends))
;;;  (define-key c-mode-map  [(tab)] 'company-complete)
;;;  (define-key c++-mode-map  [(tab)] 'company-complete)
  (eval-after-load 'company
    '(add-to-list 'company-backends 'company-irony))
  (eval-after-load 'company
    '(add-to-list 'company-backends 'company-c-headers))
  (eval-after-load 'company
                                        ;'(add-to-list 'company-c-headers-path-system "/usr/include/c++/5.4.0/"))
    '(add-to-list 'company-c-headers-path-system "/usr/include/c++/7.4.0/"))
  (message "company ... [DONE]"))

;;; Irony for code completion
(use-package irony
;  :disabled ; default [enabled]
  :ensure t
  :config
  (add-hook 'c++-mode-hook 'irony-mode)
  (add-hook 'c-mode-hook 'irony-mode)
  (add-hook 'objc-mode-hook 'irony-mode)
  (add-hook 'irony-mode-hook 'irony-cdb-autosetup-compile-options)
  (message "irony ... [DONE]"))

;;; Auto-complete basic configuration
(use-package auto-complete
  :disabled ; default [disabled]
  :ensure t
  :init
  (progn
    (ac-config-default)
    (global-auto-complete-mode t)
    )
  :config
  (message "auto-complete ... [DONE]"))

;;; Flycheck - on-the-fly-sintax checking extension
;;; This consume LARGE AMOUNTS OF MEMORY that is why it is disabled, a
;;; delay on response is introduced by the activation of this option
(use-package flycheck
  :disabled ; default [enabled]
  :ensure t
  :init
  (global-flycheck-mode t)
  :config
  (message "flycheck ... [DONE]"))

;;; Yasnippet
(use-package yasnippet
  :disabled ; default [disabled]
  :ensure t
  :init
  (yas-global-mode 1)
  :config
  (message "yasnippet ... [DONE]"))

;;; Agressive indent, automatically indents the code
(use-package aggressive-indent
  :disabled ; default [disabled]
  :ensure t
  :config
  (global-aggressive-indent-mode 1)
  (message "aggressive-indent ... [DONE]"))

;;; Expand the marked region in semantic increments (negative prefix
;;; to reduce region). Hit C-=, then only hit '=' to increment the
;;; region or '-' to reduce the marked region
(use-package expand-region
  :ensure t
  :config 
  (global-set-key (kbd "C-=") 'er/expand-region)
  (message "expand-region ... [DONE]"))

;;; iedit, allows to modify an specific text in multiple locations
;;; where it is found. Hit C-; on the word that you want to modify and
;;; start typing, notice how all the instances of the word are
;;; modified. Hit C-; when finish
(use-package iedit
  :ensure t
  :config
  (global-set-key (kbd "C-'") 'iedit-mode)
  (global-set-key (kbd "M-'") 'iedit--quit)
  (message "iedit ... [DONE]"))

;;;(message "helm-config ...")
;;; helm, a nice set of tools to search for and navigate in files
;;;(require 'helm-config)
;;;(global-set-key (kbd "C-x b") 'helm-mini)
;;;(global-set-key (kbd "C-x m") 'helm-M-x)
;;;(global-set-key (kbd "C-x C-f") 'helm-find-files)
;;;(helm-mode 1)
;;;(define-key helm-map (kbd "<tab>") 'helm-execute-persistent-action) ; rebind tab to do persistent action
;;;(define-key helm-map (kbd "C-i") 'helm-execute-persistent-action) ; make TAB works in terminal
;;;(define-key helm-map (kbd "C-z") 'helm-select-action) ; list actions using C-z
;;;(message "helm-config ... [DONE]")

;;;(message "projectile ...")
;;; Projectile and helm integration
;;;(projectile-mode)
;;;(setq projectile-completion-system 'helm)
;;;(helm-projectile-on)
;;;(setq projectile-switch-project-action 'helm-projectile) ; Bind the action after projectile-switch-project
;;;(message "projectile ... [DONE]")

;;; Enable semantic
;;;(require 'cc-mode)
(use-package cc-mode
  :ensure t
  :config
  (message "cc-mode ... [DONE]"))

;;;(require 'semantic)
(use-package semantic
  :ensure t
  :config
  (global-semanticdb-minor-mode 1)
  (global-semantic-idle-scheduler-mode 1)
  (semantic-mode 1)
  (message "semantic ... [DONE]"))

;;; Add semantic as a asuggestion backend to auto complete and hook
;;; this function to c-mode-common-hook
;;;(defun my:add-semantic-to-autocomplete()
;;;  (add-to-list 'ac-sources 'ac-source-semantic)
;;;  )
;;;(add-hook 'c-mode-common-hook 'my:add-semantic-to-autocomplete)

;;; Enable the package elpy, used for python
(use-package elpy
  ;;;:disabled ; default [enabled]
  :ensure t
  :config
  (message "elpy ... [DONE]"))

(use-package web-mode
  :ensure t
  :config
  (add-to-list 'auto-mode-alist '("\\.html?\\'" . web-mode))
  (setq web-mode-engines-alist
        '(("django"    . "\\.html\\'")))
  (setq web-mode-ac-sources-alist
        '(("css" . (ac-source-css-property))
          ("html" . (ac-source-words-in-buffer ac-source-abbrev))))
  
  (setq web-mode-enable-auto-closing t)
  (setq web-mode-enable-auto-quoting t) ; this fixes the quote problem I mentioned
  (message "web-mode ... [DONE]"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Personal emacs editor configuration
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(message "Personal emacs editor configuration ...")
;;; Inhibit the startup message, we use setq to avoid using the "'" in
;;; the first argument (in case the word inhibit-startup-message)
(setq inhibit-startup-message t)

;;; Display line numbers
(global-linum-mode t)

;;; Get rid of the tool bar
(tool-bar-mode 0)

;;; Load the prefered theme
(load-theme 'tsdh-dark)

;;; Highlight current line
(global-hl-line-mode t)

;;; Auto insert right bracket parenthesis when the left one is typed
;;;(electric-pair-mode 1)

;;; turn on highlight matching brackets when cursor is on one
(show-paren-mode 1)

;;; highlight brackets
;;;(setq show-paren-style 'parenthesis)

;;; highlight entire expression
;;;(setq show-paren-style 'expression)

;;; highlight brackets if visible, else entire expression
(setq show-paren-style 'mixed)

;;; Indentation, set the defaul value (normally is 4, other uses 2 and
;;; some others 8)
(setq-default c-basic-offset 1)

;;; Octave mode
(autoload 'octave-mode "octave-mod" nil t)
(setq auto-mode-alist
      (cons '("\\.m$" . octave-mode) auto-mode-alist))
(add-hook 'octave-mode-hook
          (lambda ()
            (abbrev-mode 1)
            (auto-fill-mode 1)
            (if (eq window-system 'x)
                (font-lock-mode 1))))

;;; Add cmake listfile names to the mode list.
(setq auto-mode-alist
      (append
       '(("CMakeLists\\.txt\\'" . cmake-mode))
       '(("\\.cmake\\'" . cmake-mode))
       auto-mode-alist))

(autoload 'cmake-mode "~/CMake/Auxiliary/cmake-mode.el" t)

(message "Personal emacs editor configuration ... [DONE]")

(message "Key bindings ...")
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Key bindings
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Set shortcut for command "goto-line"
(global-set-key (kbd "C-h g") 'goto-line)

;;; Set shortcut for revisiting a file or refresh
(global-set-key [f5] 'revert-buffer)

;;; Set shortcut for compile command
(global-set-key [f6] 'compile)

;;; Set shortcut for previous compilation error
(global-set-key [f7] 'compilation-previous-error)

;;; Set shortcut for previous compilation error
(global-set-key [f8] 'compilation-next-error)

(message "Key bindings ... [DONE]")

(message "Custom variables ...")
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Custom variables
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(column-number-mode t)
 '(doc-view-continuous t)
 '(font-use-system-font t)
 '(indent-tabs-mode nil)
 '(ispell-dictionary nil)
 '(mouse-wheel-progressive-speed nil)
 '(mouse-wheel-scroll-amount '(1 ((shift) . 1) ((control))))
 '(package-selected-packages
   '(julia-mode company-auctex gnuplot try web-mode git-timemachine elpy cmake-font-lock cmake-mode iedit expand-region company-c-headers company-irony dumb-jump ggtags markdown-mode counsel-projectile projectile auctex magit counsel undo-tree org-bullets ace-window which-key use-package))
 '(text-mode-hook '(turn-on-auto-fill text-mode-hook-identify)))

(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(default ((t (:family "Ubuntu Mono" :foundry "unknown" :slant normal :weight normal :height 113 :width normal))))
 '(aw-leading-char-face ((t (:inherit ace-jump-face-foreground :height 3.0)))))

(message "Custom variables ...  [DONE]")

(message "Finishing [DONE]")
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; init ends here
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
