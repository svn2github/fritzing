tinyMCE.init({
	"theme_advanced_toolbar_location": "top",
	"spellchecker_rpc_url": "/tinymce/spellchecker/",
	"inline_styles": "true",
	"language": "en",
	"spellchecker_languages": "+English=en,Deutsch=de",
	"apply_source_formatting": "true",
	"file_browser_callback": "djangoFileBrowser",
	"theme_advanced_buttons1": "bold,italic,|,indent,outdent,|,bullist,numlist,|,link,unlink,image,sourcecode,|,undo,redo,|,code,",
	"theme_advanced_resizing": "true",
	"theme_advanced_buttons3": "",
	"theme_advanced_statusbar_location": "bottom",
	"theme": "advanced",
	"elements": "id_body",
	"directionality": "ltr",
	"theme_advanced_resize_horizontal": "true",
	"plugins": "table,spellchecker,paste,searchreplace",
	"strict_loading_mode": 1,
	"theme_advanced_buttons2": "",
	"dialog_type": "window",
	"mode": "exact",
	setup : function(ed) {
		// Add a custom button
		ed.addButton('sourcecode', {
			title : 'Code',
			image : '/media/fritzing/img/code-button.png',
			onclick : function() {
				ed.focus();
				var content = ed.selection.getContent();
				ed.selection.setContent("<code>"+content+"</code>")
			}
		});
	}
})