
(function ($) {

Drupal.behaviors.chargelogFieldsetSummaries = {
	attach: function (context) {
		$('fieldset#edit-user', context).drupalSetSummary(function (context) {
			var searchUserStudentID = $('.form-item-uid input', context).val();
			var searchUserRealName = $('.form-item-username input', context).val();
			
			if(searchUserStudentID=="" && searchUserRealName=='')
				return Drupal.t('List orders of all users');
			else
				return Drupal.t('Search orders related to @uid @name', { '@uid': searchUserStudentID, '@name': searchUserRealName});
		});

		$('fieldset#edit-orders', context).drupalSetSummary(function (context) {
			var searchOrderNumber = $('.form-item-orderno input', context).val();
			var searchOrderStatus = $('.form-item-status select', context).val();
			var searchOrderDate = $('.form-item-orderdate select', context).val();
			var searchOrderGateway = $('.form-item-ordergateway select', context).val();
			var a = Drupal.t('All'); a = Drupal.t('Any order number'); a = Drupal.t('All date'); a = Drupal.t('All gateways');
			
			return Drupal.t('List @status orders with @orderno and @date via @gateway', {
				'@status': (searchOrderStatus==-1 ? '': $('.form-item-status select option:selected', context).text()),
				'@orderno': (searchOrderNumber=='' ? Drupal.t('Any order number') : searchOrderNumber),
				'@date': (searchOrderDate=='00000000' ? Drupal.t('All date'): $('.form-item-orderdate select option:selected', context).text()),
				'@gateway': (searchOrderGateway=='0' ? Drupal.t('All gateways'): $('.form-item-ordergateway select option:selected', context).text())
			});
		});
	}
};

})(jQuery);

(function ($) {
Drupal.behaviors.setfailListener = {
	attach: function () {
		var which;
		$("#onlinecharge-log-form input").click(function () {
		    which = $(this).attr("name");
		});
		$("#onlinecharge-log-form").submit(function () {
			if(which.match("^setfail"))
				return window.confirm(Drupal.t("Use this feature will refresh the order number. Only do this AFTER relative operations was done in payment gateway.")+"\n\n"+Drupal.t("Are you sure?"));
			else return true;
		});
	}
};
})(jQuery);

(function($) {
	$(document).ready(function() {
		jQuery("#onlinecharge-user-orders input[type=hidden][name=screenwidth]").val(window.screen.width);
	});
}(jQuery));
