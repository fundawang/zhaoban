
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
			var a = Drupal.t('All'); a = Drupal.t('Any order number'); a=Drupal.t('All date');
			
			return Drupal.t('List @status orders with @orderno and @date', {
				'@status': (searchOrderStatus==-1 ? '': $('.form-item-status select option:selected', context).text()),
				'@orderno': (searchOrderNumber=='' ? Drupal.t('Any order number') : searchOrderNumber),
				'@date': (searchOrderDate=='00000000' ? Drupal.t('All date'): $('.form-item-orderdate select option:selected', context).text())
			});
		});
	}
};

})(jQuery);
