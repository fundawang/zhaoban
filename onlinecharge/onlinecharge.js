
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
			
			if(searchOrderNumber=="" && searchOrderStatus=='')
				return Drupal.t('List all orders');
			else if(searchOrderNumber=="")
				return searchOrderStatus==-1?Drupal.t('Search all orders'):Drupal.t('Search orders which are @status', { '@status': $('.form-item-status select option:selected', context).text()});
			else
				return searchOrderStatus==-1?Drupal.t('Search orders with number @orderno', { '@orderno': searchOrderNumber}):Drupal.t('Search orders which are @status with number @orderno', { '@orderno': searchOrderNumber, '@status': $('.form-item-status select option:selected', context).text()});
		});
	}
};

})(jQuery);
