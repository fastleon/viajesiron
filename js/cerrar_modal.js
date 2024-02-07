(function ($) {
    Drupal.behaviors.cerrarModal = {
      attach: function (context, settings) {
        Drupal.myModule = Drupal.myModule || {};
        Drupal.myModule.cerrar_modal = function () {
            Drupal.CTools.Modal.dismiss();
        };
      }
    };
  })(jQuery);