(function($) {
    $(document).ready(function() {
      $('#edit-filtro-remisiones').keypress(function(event) {
        if (event.keyCode == 13) {
          event.preventDefault();
          $(this).trigger('change');
        }
      });
    });
  })(jQuery);